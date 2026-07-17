#Tablas y estructuras.
from dataclasses import dataclass

@dataclass(frozen=True)
class IdentityDevice:
    id_dispositivo:str      # identificador del dispositivo (MAC)
    dispositivo: str        # nombre
