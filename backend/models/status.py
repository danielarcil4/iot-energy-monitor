#Tablas y estructuras.
from dataclasses import dataclass

@dataclass(frozen=True)
class statusDevice:
    uptime: int          # tiempo del dispositivo encendido
    wifi_rssi: int       # rssi de la red wifi
    free_heap: int       # cantidad de memoria heap libre en bytes
    messages_sent: int   # cantidad de mensajes enviados
    online: bool         # estado del dispositivo 1: ONLINE 0: OFFLINE