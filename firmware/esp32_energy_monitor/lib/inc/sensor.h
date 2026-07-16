#ifndef SENSOR_H
#define SENSOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "esp_err.h"

#define SIZE_BUFFER_TIME 24 // Tamaño del buffer para la hora en formato HH:MM:SS

typedef struct sensor_t sensor_t;

typedef enum {
    SENSOR_TYPE_TEMPERATURE,
    SENSOR_TYPE_HUMIDITY,
    SENSOR_TYPE_PRESSURE,
    SENSOR_TYPE_LIGHT,
    SENSOR_TYPE_CUSTOM
} sensor_type_t; // Enumeración para los tipos de sensores

typedef enum {
    SENSOR_UNIT_CELSIUS,
    SENSOR_UNIT_PERCENTAGE,
    SENSOR_UNIT_PASCAL,
    SENSOR_UNIT_LUMEN,
    SENSOR_UNIT_CUSTOM
} sensor_unit_t; // Enumeración para los tipos de sensores

typedef struct {
    float value;                      // Valor del sensor 
    char timestamp[SIZE_BUFFER_TIME]; // tiempo en formato date:HH:MM:SS cuando se tomó la lectura del sensor
} sensor_data_t; // Estructura para almacenar los datos del sensor

typedef esp_err_t (*sensor_read_fn_t)(sensor_t *sensor, sensor_data_t *data); // Definición del tipo de función para leer datos del sensor que se cree

struct sensor_t {
    uint8_t id_sensor;      // Identificador único del sensor
    sensor_type_t type;     // Tipo de sensor (temperatura, humedad, etc.)
    sensor_unit_t unit;     // Unidad de medida del sensor (Celsius, porcentaje, etc.)
    const char *mqtt_topic; // Tema MQTT al que se publicarán los datos del sensor
    sensor_read_fn_t read;  // Apuntador a función para leer los datos del sensor
}; // Estructura para representar un sensor

extern sensor_t *sensors[]; // Arreglo de punteros a sensores
extern const size_t sensors_count; // Número de sensores en el arreglo

const char *sensor_unit_to_string(sensor_unit_t unit);  // Función para convertir la unidad del sensor a una cadena de texto
const char *sensor_type_to_string(sensor_type_t type);  // Función para convertir el tipo del sensor a una cadena de texto
void get_time(char *time_buffer);                       // Función para obtener la hora actual en formato HH:MM:SS y almacenarla en el buffer proporcionado

#endif // SENSOR_H
