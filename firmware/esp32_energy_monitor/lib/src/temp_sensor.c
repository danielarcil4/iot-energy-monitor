#include "temp_sensor.h"
#include "esp_random.h"

#define MQTT_TOPIC_HABITACION_1 "esp32/sensor/temperatura/habitacion/1"
#define MQTT_TOPIC_HABITACION_2 "esp32/sensor/temperatura/habitacion/2"

static esp_err_t temperature_sensor_read(sensor_t *sensor, sensor_data_t *data) {
    (void)sensor;

    data->value = esp_random() % 40; // Genera un número aleatorio entre 0 y 40
    get_time(data->timestamp);

    return ESP_OK;
}

sensor_t temperature_sensor_habitacion_1 = {
    .id_sensor = 1,
    .type = SENSOR_TYPE_TEMPERATURE,
    .unit = SENSOR_UNIT_CELSIUS,
    .mqtt_topic = MQTT_TOPIC_HABITACION_1,
    .read = temperature_sensor_read,
};

sensor_t temperature_sensor_habitacion_2 = {
    .id_sensor = 2,
    .type = SENSOR_TYPE_TEMPERATURE,
    .unit = SENSOR_UNIT_CELSIUS,
    .mqtt_topic = MQTT_TOPIC_HABITACION_2,
    .read = temperature_sensor_read,
};