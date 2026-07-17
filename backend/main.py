from fastapi import FastAPI
from contextlib import asynccontextmanager
from .mqtt.mqtt_client import build_client
from backend.database.sensor_repository import initialize_database
from .config import MQTT_BROKER_HOST, MQTT_BROKER_PORT
from .api.routes.measurments import router as router_measurment
from .api.routes.devices import router as router_device
from .api.routes.health import router as router_health
from .services.statistics import router as router_statistics


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

# incluimos routes
app.include_router(router_measurment)
app.include_router(router_device)
app.include_router(router_health)
app.include_router(router_statistics)

@app.get("/")
def read_root():
    return {"message": "Hello, FastAPI!"}


