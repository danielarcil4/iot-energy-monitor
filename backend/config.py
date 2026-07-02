import os

MQTT_BROKER_HOST = os.getenv("MQTT_BROKER_HOST", "localhost")
MQTT_BROKER_PORT = int(os.getenv("MQTT_BROKER_PORT", "1883"))
MQTT_TOPIC = os.getenv("MQTT_TOPIC", "esp32/sensor_temperatura")
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_DATABASE_PATH = os.path.join(PROJECT_ROOT, "database", "sensors_data.db")
DATABASE_PATH = os.getenv("SENSOR_DATABASE_PATH", DEFAULT_DATABASE_PATH)