#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t id_device[6];
    char name[32];
} device_identity_t;

typedef struct {
    uint32_t uptime;
    int wifi_rssi;
    uint32_t free_heap;
    uint32_t messages_sent;
    bool online;
} device_status_t;

const device_identity_t *get_device_identity(void);
const device_status_t *get_device_status(void);
const char *get_mqtt_topic();

#endif