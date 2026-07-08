
from typing import Any
import paho.mqtt.client as mqtt
from .database import save_reading, parse_json_payload
from .config import MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_TOPICS

def on_connect(client: mqtt.Client, userdata: Any, flags: dict[str, Any], rc: int) -> None:
    if rc == 0:
        print(f"Conectado al broker MQTT en {MQTT_BROKER_HOST}:{MQTT_BROKER_PORT}")
        client.subscribe([(topic, 0) for topic in MQTT_TOPICS])
        print(f"Escuchando mensajes en los topicos: {', '.join(MQTT_TOPICS)}")
        return

    print(f"No se pudo conectar al broker MQTT. Codigo de retorno: {rc}")


def on_message(client: mqtt.Client, userdata: Any, message: mqtt.MQTTMessage) -> None:
    payload = message.payload.decode("utf-8", errors="replace")
    reading = parse_json_payload(payload)

    if reading is None:
        print(f"Mensaje ignorado en {message.topic}: {payload}")
        return

    save_reading(reading)
    print(
        "Lectura guardada: "
        f"Tipo de sensor: {reading.type}, "
        f"Valor: {reading.data}, "
        f"Unidad: {reading.unit}, "
        f"Timestamp: {reading.timestamp}"
    )


def build_client() -> mqtt.Client:
    if mqtt is None:
        raise RuntimeError(
            "Error"
        )

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    return client
    
    
