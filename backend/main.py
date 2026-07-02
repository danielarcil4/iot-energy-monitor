# Levanta FastAPI.
from database import initialize_database
from mqtt_client import build_client
from config import MQTT_BROKER_HOST, MQTT_BROKER_PORT

# Inicializa la base de datos y el cliente MQTT
initialize_database()
client = build_client()
client.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT, keepalive=60)

client.loop_forever()