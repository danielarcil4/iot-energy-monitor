#ifndef DEVICE_H
#define DEVICE_H
#include <stdint.h>


uint8_t device_get_id(void);       // Función para obtener el ID del dispositivo
const char* device_get_name(void); // Función para obtener el nombre del dispositivo

#endif // DEVICE_H