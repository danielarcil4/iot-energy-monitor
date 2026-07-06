#include "sensor.h"
#include <time.h>
#include "temp_sensor.h"

void get_time(char *time_buffer) {
    time_t now = time(NULL);
    struct tm timeinfo;

    localtime_r(&now, &timeinfo);

    strftime(time_buffer, SIZE_BUFFER_TIME, "%H:%M:%S", &timeinfo); 
}

const char *sensor_type_to_string(sensor_type_t type) {
    switch (type) {
        case SENSOR_TYPE_TEMPERATURE:
            return "temperatura";
        case SENSOR_TYPE_HUMIDITY:
            return "humedad";
        case SENSOR_TYPE_PRESSURE:
            return "presión";
        case SENSOR_TYPE_LIGHT:
            return "luz";
        default:
            return "unknown";
    }
}

const char *sensor_unit_to_string(sensor_unit_t unit) {
    switch (unit) {
        case SENSOR_UNIT_CELSIUS:
            return "celsios";
        case SENSOR_UNIT_PERCENTAGE:
            return "porcentaje";
        case SENSOR_UNIT_PASCAL:
            return "pascal";
        case SENSOR_UNIT_LUMEN:
            return "lumen";
        default:
            return "unknown";
    }
} 

sensor_t *sensors[] = {
    &temperature_sensor_habitacion_1,
    &temperature_sensor_habitacion_2,
};

const size_t sensors_count = sizeof(sensors) / sizeof(sensors[0]);
