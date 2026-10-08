#ifndef MQTT_SERVICE_H
#define MQTT_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Creates the MQTT client and prepares the broker address.
  * @return None
  * @note   Must be called after MX_LWIP_Init(), from the task that drives lwIP.
  */
void mqttServiceInit(void);

/**
  * @brief  Drives the MQTT connection: connects, retries and publishes status.
  * @return None
  * @note   Must be called periodically from the same task as MX_LWIP_Process().
  *         lwIP runs with NO_SYS=1, so it is not safe to call from another task.
  */
void mqttServiceProcess(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_SERVICE_H */
