#include "device.h"
#include <stdio.h>
#include <string.h>     // Necesario para strcmp y snprintf
#include "esp_system.h"
#include "esp_mac.h"

// 1. Estructura estática global (viva para siempre), pero SIN 'const' aquí 
// para que las funciones de este archivo puedan rellenarla.
static data_device_t dispositivo = {0};

// Variable interna para saber si ya se inicializaron los datos
static bool inicializado = false;

// Función auxiliar para leer los datos antes de hacerlos constantes
data_device_t init_data_device(void) {
    data_device_t info_temporal = {0};

    // 1. Leer la MAC (guarda 6 bytes numéricos)
    esp_read_mac(info_temporal.id, ESP_MAC_WIFI_STA);
    
    // 2. Verificar si el nombre tiene el valor por defecto de Kconfig
    // NOTA: Reemplaza "nombre_por_defecto" por el valor 'default' exacto que pusiste en tu Kconfig
    if (strcmp(CONFIG_NAME, "nombre_del_dispositivo") == 0) {
        
        // El último byte de la MAC (info_temporal.id[5]) contiene los dos últimos caracteres hexadecimales.
        // Usamos snprintf para formatear el texto seguro "dispositivo_XX" (necesita mínimo 10 bytes de espacio)
        snprintf(info_temporal.name, sizeof(info_temporal.name), "dispositivo_%02X", info_temporal.id[5]);
        
    } else {
        // Si el usuario sí configuró un nombre en el Kconfig, usamos ese
        snprintf(info_temporal.name, sizeof(info_temporal.name), "%s", CONFIG_NAME);
    }

    inicializado = true;

    return info_temporal;
}

void device_init(void) {
    if (inicializado) return; // Evita inicializar dos veces
    dispositivo = init_data_device();   // Inizializar los datos
}

const data_device_t* get_data_device(void) {
    if (!inicializado) {
        device_init();
    }
    return &dispositivo;
}