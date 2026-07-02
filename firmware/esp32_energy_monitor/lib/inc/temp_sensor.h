#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <stdint.h>
#include <stdio.h>

#define SIZE_BUFFER_TIME 9 // Tamaño del buffer para la hora en formato HH:MM:SS


typedef struct {
    float temperature;    // Temperatura en grados Celsius
    char tiempo_actual[SIZE_BUFFER_TIME]; // HH:MM:SS
} datos_t;

extern datos_t datos; // Declaración de la variable global

void get_data_temperature_sensor(void); // Función para obtener datos del sensor de temperatura

#endif // TEMP_SENSOR_H
