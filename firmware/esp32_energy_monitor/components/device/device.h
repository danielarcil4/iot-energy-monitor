#ifndef DEVICE_H
#define DEVICE_H
#include <stdint.h>

typedef struct {
    uint8_t id[6];  // mac del dispositivo
    char name[32];  // nombre del dispositivo
} data_device_t;    // estructura para almacenar los datos de un dispositivo

const data_device_t* get_data_device(void); // devuelve la estructura con los datos del dispositivo 

#endif // DEVICE_H