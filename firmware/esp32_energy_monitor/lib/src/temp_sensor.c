#include "temp_sensor.h"
#include "esp_random.h"
#include <time.h>

datos_t datos; // Definición de la variable global

void get_time(char *time_buffer) {
    time_t now = time(NULL);
    struct tm timeinfo;

    localtime_r(&now, &timeinfo);

    strftime(time_buffer, SIZE_BUFFER_TIME, "%H:%M:%S", &timeinfo); 
}

void get_data_temperature_sensor(void) {
    datos.temperature = esp_random() % 40; // Genera un número aleatorio entre 0 y 40
    get_time(datos.tiempo_actual); // Obtiene la hora actual
}


