#Tablas y estructuras.
from dataclasses import dataclass

@dataclass(frozen=True)
class Sensor:
    id: int          # identificador del sensor
    type_sensor: str # tipo de sensor ejemplo: Temperatura, Humedad, etc.
    data: float      # valor del sensor
    unit: str        # unidad de medida
    timestamp: str   # marca de tiempo