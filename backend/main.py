from fastapi import FastAPI
from contextlib import asynccontextmanager
from .mqtt_client import build_client
from .database import initialize_database, get_measurements, get_latest_measurement
from .config import MQTT_BROKER_HOST, MQTT_BROKER_PORT

@asynccontextmanager
async def lifespan(app: FastAPI):
    initialize_database()

    mqtt_client = build_client()
    mqtt_client.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT, keepalive=60)
    mqtt_client.loop_start()
    app.state.mqtt_client = mqtt_client

    yield

    mqtt_client.loop_stop()
    mqtt_client.disconnect()


app = FastAPI(lifespan=lifespan)


@app.get("/")
def read_root():
    return {"message": "Hello, FastAPI!"}

@app.get("/measurements")
def read_measurements():
    measurements = get_measurements()
    return {"message": measurements}

@app.get("/measurements/latest")
def read_latest_measurements():
    latest_measurement = get_latest_measurement()
    return {"message": latest_measurement}

@app.get("/devices")
def read_devices():
    # Verify life of devices based on publiher
    return {"message": "List of devices"}