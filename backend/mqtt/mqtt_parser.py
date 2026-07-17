import json
from typing import Any
from backend.models.sensor import Sensor
from backend.models.device import IdentityDevice
from backend.models.status import statusDevice


def parse_json_payload_sensor(payload: str) -> Sensor | None:
    try:
        data: dict[str, Any] = json.loads(payload)
    except json.JSONDecodeError:
        return None
    if data is None:
        return None
    try:
        return Sensor(
            type             =  str(data.get("type")),
            value            =  float(data.get("value")),
            unit             =  str(data.get("unit")),
            timestamp        =  str(data.get("timestamp")),
        )
    except (TypeError, ValueError):
        return None
    
def parse_json_payload_device(payload: str) -> IdentityDevice | None:
    try:
        data: dict[str, Any] = json.loads(payload)
    except json.JSONDecodeError:
        return None
    if data is None:
        return None
    try:
        return IdentityDevice(
            id_dispositivo = str(data["id_dispositivo"]),
            dispositivo = str(data["dispositivo"])
        )
    except (TypeError, ValueError):
        return None
    
def parse_json_payload_status(payload: str) -> statusDevice | None:
    try:
        data: dict[str, Any] = json.loads(payload)
    except json.JSONDecodeError:
        return None
    if data is None:
        return None
    try:
        return statusDevice(
            uptime        =   int(data.get("uptime")),
            wifi_rssi     =   int(data.get("wifi_rssi")),
            free_heap     =   int(data.get("free_heap")),
            messages_sent =   int(data.get("messages_sent")),
            online        =   bool(data.get("status", "OK"))
        )
    except (TypeError, ValueError):
        return None