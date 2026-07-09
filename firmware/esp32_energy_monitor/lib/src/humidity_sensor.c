#include "humidity_sensor.h"
#include "esp_random.h"

static esp_err_t humidity_sensor_read(sensor_t *sensor, sensor_data_t *data) {
    (void)sensor;

    data->value = esp_random() % 100 + 10; // Genera un número aleatorio entre 10 y 100
    get_time(data->timestamp);             // Obtiene el tiempo en formato HH:MM:SS

    return ESP_OK;
}

sensor_t humidity_sensor_sala = {
    .id_sensor = 3,
    .type = SENSOR_TYPE_HUMIDITY,
    .unit = SENSOR_UNIT_PERCENTAGE,
    .mqtt_topic = "esp32/sensor/humedad/sala",
    .read = humidity_sensor_read,
};