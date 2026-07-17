#include "mqtt.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "sensor.h"
#include "device.h"
#include "sntp_time.h"

#include <stdio.h>
#include <time.h>

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client;

// Prototipo de la tarea
void mqtt_publish_task(void *pvParameters);

// Manejador de eventos MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado al bróker MQTT");
            xTaskCreate(mqtt_publish_task, "mqtt_publish_task", 4096,NULL, 5, NULL);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Desconectado del bróker MQTT");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "Mensaje enviado, ID: %d", event->msg_id);
            break;            
        default:
            break;
    }
}

static void wifi_ip_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    ESP_LOGI(TAG, "Conectado. Dirección IP: " IPSTR, IP2STR(&event->ip_info.ip));
    // Iniciar sntp
    init_sntp();
    // Iniciamos MQTT solo cuando ya tenemos IP y la hora
    esp_mqtt_client_start(client);
}

void mqtt_publish_task(void *pvParameters) {
    const device_identity_t *identity = get_device_identity();
    const char *device_topic = get_mqtt_topic();
    char payload[256];

    while (1) {
        const device_status_t *status = get_device_status();
        snprintf(payload, sizeof(payload),
                    "{\"id dispositivo\": \"%02X:%02X:%02X:%02X:%02X:%02X\", \"dispositivo\": \"%s\", \"status\": \"%s\", \"uptime\": \"%ld\", \"wifi rssi\": \"%d\", \"free heap\": \"%ld\", \"Numero de mensajes\": \"%ld\"}",
                    identity->id_device[0],   // primer byte del id 
                    identity->id_device[1],   // segundo byte del id 
                    identity->id_device[2],   // tercer byte del id 
                    identity->id_device[3],   // cuarto byte del id 
                    identity->id_device[4],   // quinto byte del id 
                    identity->id_device[5],   // sexto byte del id 
                    identity->name,           // nombre del dispositivo
                    "online",                 // status del dispositivo online o offline
                    status->uptime,           // tiempo de encendido del dispositivo
                    status->wifi_rssi,        // decibeles del wifi
                    status->free_heap,        // cantidad de memoria libre en bytes
                    status->messages_sent);   // cantidad de mensajes enviados

        esp_mqtt_client_publish(client, device_topic, payload, 0, 1, 0);
        
        //limpiamos payload
        memset(payload, 0x00, sizeof(payload));

        for (size_t i = 0; i < sensors_count; i++) {
            sensor_t *sensor = sensors[i];
            sensor_data_t data;

            if (sensor->read(sensor, &data) != ESP_OK) {
                ESP_LOGE(TAG, "Error leyendo sensor: %s", sensor_type_to_string(sensor->type));
                continue;
            }
            snprintf(payload, sizeof(payload),
                    "{\"id_sensor\": %d, \"type\": \"%s\", \"value\": \"%.2f\", \"unit\": \"%s\", \"timestamp\": \"%s\"}",
                    sensor->id_sensor,
                    sensor_type_to_string(sensor->type),
                    data.value,
                    sensor_unit_to_string(sensor->unit),
                    data.timestamp); 

            // Suscripcion a los topicos para el envio de datos
            esp_mqtt_client_publish(client, sensor->mqtt_topic, payload, 0, 1, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(5000)); 
    }
}

void mqtt_init(void){
    char mqtt_broker_uri[128];
    sprintf(mqtt_broker_uri, "mqtt://%s:%d", CONFIG_MQTT_BROKER, CONFIG_MQTT_BROKER_PORT);

    // Configurar cliente MQTT (pero no lo iniciamos todavía)
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = mqtt_broker_uri,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);

    if (client == NULL) {
        ESP_LOGE("MQTT", "¡Error al inicializar el cliente MQTT!");
    }
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, MQTT_EVENT_CONNECTED, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, MQTT_EVENT_DISCONNECTED, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, MQTT_EVENT_PUBLISHED, mqtt_event_handler, NULL));

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_ip_handler, NULL));

}

