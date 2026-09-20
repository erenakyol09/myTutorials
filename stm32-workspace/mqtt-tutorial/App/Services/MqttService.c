#include "MqttService.h"

#include "main.h"
#include "ApplicationWrapper.h"
#include "lwip/apps/mqtt.h"
#include "lwip/netif.h"
#include "usbd_cdc_if.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP          "192.168.10.1"
#define MQTT_CLIENT_ID          "stm32f767"
#define MQTT_TOPIC_STATUS       "stm32/status"
#define MQTT_TOPIC_COMMAND      "stm32/cmd"
#define MQTT_COMMAND_LED_ON     "ledOn"
#define MQTT_COMMAND_LED_OFF    "ledOff"
#define MQTT_KEEP_ALIVE_SEC     60U
#define MQTT_PUBLISH_PERIOD_MS  5000U
#define MQTT_RETRY_PERIOD_MS    5000U
#define MQTT_PAYLOAD_MAX_LEN    64U
#define MQTT_TOPIC_MAX_LEN      64U
#define MQTT_LOG_MAX_LEN        96U

static mqtt_client_t *mqttClient = NULL;
static ip_addr_t brokerAddress;
static uint32_t lastPublishTick = 0U;
static uint32_t lastRetryTick = 0U;
static uint32_t publishCounter = 0U;
static char incomingTopic[MQTT_TOPIC_MAX_LEN];

static const struct mqtt_connect_client_info_t mqttClientInfo = {
    .client_id = MQTT_CLIENT_ID,
    .keep_alive = MQTT_KEEP_ALIVE_SEC
};

static void mqttLog(const char *format, ...);
static void mqttIncomingPublishCallback(void *arg, const char *topic, u32_t totLen);
static void mqttIncomingDataCallback(void *arg, const u8_t *data, u16_t len, u8_t flags);
static void mqttSubscribeCallback(void *arg, err_t err);
static void mqttConnectionCallback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

void mqttServiceInit(void)
{
    if (ipaddr_aton(MQTT_BROKER_IP, &brokerAddress) == 0)
    {
        mqttLog("MQTT invalid broker address\r\n");
        return;
    }

    mqttClient = mqtt_client_new();
    if (mqttClient == NULL)
    {
        mqttLog("MQTT client allocation failed\r\n");
        return;
    }

    /* Backdate the retry timer so the first connect attempt is not delayed */
    lastRetryTick = HAL_GetTick() - MQTT_RETRY_PERIOD_MS;
}

void mqttServiceProcess(void)
{
    if ((mqttClient == NULL) || (netif_default == NULL) || (netif_is_link_up(netif_default) == 0))
    {
        return;
    }

    if (mqtt_client_is_connected(mqttClient) == 0U)
    {
        if ((HAL_GetTick() - lastRetryTick) >= MQTT_RETRY_PERIOD_MS)
        {
            err_t err;

            lastRetryTick = HAL_GetTick();
            err = mqtt_client_connect(mqttClient, &brokerAddress, MQTT_PORT,
                                      mqttConnectionCallback, NULL, &mqttClientInfo);
            if (err != ERR_OK)
            {
                mqttLog("MQTT connect failed (%d)\r\n", (int)err);
            }
        }
        return;
    }

    if ((HAL_GetTick() - lastPublishTick) >= MQTT_PUBLISH_PERIOD_MS)
    {
        char payload[MQTT_PAYLOAD_MAX_LEN];
        int length;
        err_t err;

        lastPublishTick = HAL_GetTick();
        length = snprintf(payload, sizeof(payload), "uptime=%lus count=%lu",
                          (unsigned long)(HAL_GetTick() / 1000U), (unsigned long)publishCounter);
        publishCounter++;

        err = mqtt_publish(mqttClient, MQTT_TOPIC_STATUS, payload, (u16_t)length, 0, 0, NULL, NULL);
        if (err != ERR_OK)
        {
            mqttLog("MQTT publish failed (%d)\r\n", (int)err);
        }
    }
}

/**
  * @brief  Formats a line and sends it over the USB CDC virtual COM port.
  * @param  format printf-style format string
  * @return None
  * @note   Best effort: the line is dropped if the previous CDC transfer is still in flight.
  */
static void mqttLog(const char *format, ...)
{
    char line[MQTT_LOG_MAX_LEN];
    va_list args;
    int length;

    va_start(args, format);
    length = vsnprintf(line, sizeof(line), format, args);
    va_end(args);

    if (length <= 0)
    {
        return;
    }

    if ((size_t)length > (sizeof(line) - 1U))
    {
        length = (int)(sizeof(line) - 1U);
    }

    (void)CDC_Transmit_FS((uint8_t *)line, (uint16_t)length);
}

static void mqttIncomingPublishCallback(void *arg, const char *topic, u32_t totLen)
{
    (void)arg;
    (void)totLen;

    (void)snprintf(incomingTopic, sizeof(incomingTopic), "%s", topic);
}

/* Payloads longer than the lwIP buffer arrive in several calls, each logged on its own line */
static void mqttIncomingDataCallback(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    char payload[MQTT_PAYLOAD_MAX_LEN];
    u16_t copyLength = len;

    (void)arg;
    (void)flags;

    if (copyLength > (u16_t)(sizeof(payload) - 1U))
    {
        copyLength = (u16_t)(sizeof(payload) - 1U);
    }

    (void)memcpy(payload, data, copyLength);
    payload[copyLength] = '\0';

    if (strcmp(payload, MQTT_COMMAND_LED_ON) == 0)
    {
        App_SetLed(true);
        mqttLog("MQTT rx [%s] %s, LED on\r\n", incomingTopic, payload);
    }
    else if (strcmp(payload, MQTT_COMMAND_LED_OFF) == 0)
    {
        App_SetLed(false);
        mqttLog("MQTT rx [%s] %s, LED off\r\n", incomingTopic, payload);
    }
    else
    {
        mqttLog("MQTT rx [%s] %s, unknown command\r\n", incomingTopic, payload);
    }
}

static void mqttSubscribeCallback(void *arg, err_t err)
{
    (void)arg;

    if (err == ERR_OK)
    {
        mqttLog("MQTT subscribed to %s\r\n", MQTT_TOPIC_COMMAND);
    }
    else
    {
        mqttLog("MQTT subscribe failed (%d)\r\n", (int)err);
    }
}

static void mqttConnectionCallback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    (void)arg;

    if (status == MQTT_CONNECT_ACCEPTED)
    {
        mqttLog("MQTT connected to %s\r\n", MQTT_BROKER_IP);
        mqtt_set_inpub_callback(client, mqttIncomingPublishCallback, mqttIncomingDataCallback, NULL);
        (void)mqtt_sub_unsub(client, MQTT_TOPIC_COMMAND, 0, mqttSubscribeCallback, NULL, 1);
    }
    else
    {
        mqttLog("MQTT connection lost (%d)\r\n", (int)status);
    }
}
