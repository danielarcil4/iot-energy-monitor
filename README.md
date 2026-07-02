# IoT Energy Monitoring Platform

Sistema IoT para adquisición, almacenamiento y consulta de variables eléctricas utilizando ESP32, MQTT y FastAPI.

## Características

- Adquisición de datos desde ESP32
- Comunicación MQTT
- Almacenamiento en SQLite
- API REST con FastAPI
- Documentación Swagger

## Arquitectura actual

ESP32
 ↓
MQTT
 ↓
MQTT EXPLORER (VISUALIZACIÓN)

## Requisitos

- ESP-IDF v6.1
- Python 3.14.6
- Mosquitto MQTT Broker
- MQTT Explorer (opcional)

## Instalación

1. Clonar el repositorio.
2. Crear el entorno virtual.
3. Instalar dependencias de Python.
4. Configurar la IP del broker MQTT.
5. Compilar el firmware.
6. Ejecutar el backend.

# MQTT Topics

 - esp32/sensor_temperatura: temperatura del sensor.

## Ejecución

1. Iniciar el broker Mosquitto.
2. Abrir MQTT Explorer y conectarse al broker.
3. Compilar y cargar el firmware en el ESP32.
4. Abrir el monitor serie para verificar la conexión WiFi y la publicación de mensajes.
5. Verificar en MQTT Explorer que los mensajes llegan al tópico configurado.

## Estado

- [x] Proyecto creado
- [x] WiFi
- [x] MQTT
- [ ] Base de datos SQLite
- [ ] Simulación de sensores
- [ ] Backend FastAPI