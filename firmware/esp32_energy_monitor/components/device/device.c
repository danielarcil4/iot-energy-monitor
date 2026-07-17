#include "device.h"
#include <stdio.h>
#include <string.h>     // Necesario para strcmp y snprintf
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_timer.h"
#include "esp_wifi.h"

// 1. Estructuras estáticas globales para no tener que pasar por parámetros
static device_identity_t identity = {0};
static device_status_t status = {0};

// Variable interna para saber si ya se inicializaron los datos
static bool inicializado = false;

// Variable interna para saber cuantos mensajes se han enviado
uint32_t messages_sent = 0;

// Variable interna que contiene el topico de MQTT para enviar los datos
char topic[44] = {0};


// Función auxiliar para leer los datos antes de hacerlos constantes
device_identity_t init_device_identity(void) {
    // 1. Leer la MAC (guarda 6 bytes numéricos)
    esp_read_mac(identity.id_device, ESP_MAC_WIFI_STA);
    
    // 2. Verificar si el nombre tiene el valor por defecto de Kconfig
    // NOTA: Reemplaza "nombre_por_defecto" por el valor 'default' exacto que pusiste en tu Kconfig
    if (strcmp(CONFIG_NAME, "nombre_del_dispositivo") == 0) {
        
        // El último byte de la MAC (info_temporal.id_device[5]) contiene los dos últimos caracteres hexadecimales.
        // Usamos snprintf para formatear el texto seguro "dispositivo_XX" (necesita mínimo 10 bytes de espacio)
        snprintf(identity.name, sizeof(identity.name), "dispositivo_%02X", identity.id_device[5]);
        
    } else {
        // Si el usuario sí configuró un nombre en el Kconfig, usamos ese
        snprintf(identity.name, sizeof(identity.name), "%s", CONFIG_NAME);
    }

    snprintf(topic, sizeof(topic), "esp32/device/%02X:%02X:%02X:%02X:%02X:%02X/status", identity.id_device[0], identity.id_device[1], identity.id_device[2], identity.id_device[3], identity.id_device[4], identity.id_device[5]);

    inicializado = true;

    return identity;
}

const device_status_t *get_device_status(void){
    status.uptime = esp_timer_get_time() / 1000; // tiempo en segundos
    esp_wifi_sta_get_rssi(&status.wifi_rssi);  // RSSI de la red Wi-Fi
    status.free_heap = esp_get_free_heap_size(); // espacio libre en el heap
    status.messages_sent = ++messages_sent;      // número de mensajes enviados

    return &status;
}

const char *get_mqtt_topic(){
    return topic; 
}

const device_identity_t* get_device_identity(void) {
    if (!inicializado) {
        identity = init_device_identity();   // Inizializar los datos
    }
    return &identity;
}