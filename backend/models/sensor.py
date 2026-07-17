#Tablas y estructuras.
from dataclasses import dataclass

@dataclass(frozen=True)
class Sensor:
    type: str               # tipo de sensor ejemplo: Temperatura, Humedad, etc.
    value: float             # valor del sensor
    unit: str               # unidad de medida
    timestamp: str          # marca de tiempo