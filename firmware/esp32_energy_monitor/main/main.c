#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "temp_sensor.h"

static const char *TAG = "MQTT_PUB";
static esp_mqtt_client_handle_t client;

// Prototipo de la tarea
void mqtt_publish_task(void *pvParameters);

// Manejador de eventos Wi-Fi y de IP
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Reconectando al Wi-Fi...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "Conectado. Dirección IP: " IPSTR, IP2STR(&event->ip_info.ip));
        // Iniciamos MQTT solo cuando ya tenemos IP
        esp_mqtt_client_start(client);
    }
}

// Manejador de eventos MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado al bróker MQTT");
            xTaskCreate(mqtt_publish_task, "mqtt_publish_task", 4096, NULL, 5, NULL);
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

void mqtt_publish_task(void *pvParameters) {
    char payload[64];

    while (1) {
        get_data_temperature_sensor();
        sprintf(payload, "{\"temperature\": \"%.2f\", \"timestamp\": \"%s\"}", datos.temperature, datos.tiempo_actual);
        esp_mqtt_client_publish(client, "esp32/sensor_temperatura", payload, 0, 1, 0);
        vTaskDelay(pdMS_TO_TICKS(5000)); 
    }
}

void wifi_init_sta(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

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
    if(esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL) != ESP_OK) {
        ESP_LOGE("MQTT", "¡Error al registrar el manejador de eventos MQTT!");
    }
    

    // Iniciar Wi-Fi (este activará el MQTT automáticamente al conectar)
    wifi_init_sta();
}
