#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H

/**
  * Network profile switch.
  *
  * 1: the address is requested from a DHCP server. Use this when the board is plugged into a
  *    router or switch that runs a DHCP server.
  * 0: the static configuration below is used. Use this when the board is cabled directly to the
  *    development machine, where there is no DHCP server.
  *
  * This header is also included from lwipopts.h, so changing the switch reconfigures both the
  * lwIP stack and the application.
  */
#define NETWORK_USE_DHCP 1

/* Static address, used only when NETWORK_USE_DHCP is 0 */
#define NETWORK_STATIC_IP_0 192
#define NETWORK_STATIC_IP_1 168
#define NETWORK_STATIC_IP_2 10
#define NETWORK_STATIC_IP_3 2

#define NETWORK_STATIC_NETMASK_0 255
#define NETWORK_STATIC_NETMASK_1 255
#define NETWORK_STATIC_NETMASK_2 255
#define NETWORK_STATIC_NETMASK_3 0

#define NETWORK_STATIC_GATEWAY_0 0
#define NETWORK_STATIC_GATEWAY_1 0
#define NETWORK_STATIC_GATEWAY_2 0
#define NETWORK_STATIC_GATEWAY_3 0

/**
  * Broker address, which differs per profile: on a router it is the LAN address of the machine
  * running Mosquitto (check it with `ipconfig getifaddr en0`), on a direct cable it is the address
  * assigned to the USB Ethernet adapter.
  */
#if NETWORK_USE_DHCP
#define NETWORK_MQTT_BROKER_IP "192.168.1.103"
#else
#define NETWORK_MQTT_BROKER_IP "192.168.10.1"
#endif

/** Writes four octets into one of the lwIP address arrays. */
#define NETWORK_SET_ADDR(dst, a, b, c, d) \
    do {                                  \
        (dst)[0] = (a);                   \
        (dst)[1] = (b);                   \
        (dst)[2] = (c);                   \
        (dst)[3] = (d);                   \
    } while (0)

#endif /* NETWORK_CONFIG_H */
