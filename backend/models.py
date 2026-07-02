#Tablas y estructuras.
from dataclasses import dataclass

@dataclass(frozen=True)
class TemperatureSensor:
    temperature: float
    timestamp: str