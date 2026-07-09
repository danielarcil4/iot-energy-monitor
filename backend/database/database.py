#Conexión SQLite.
import json
import sqlite3
from typing import Any
from backend.models.models import Sensor
from backend.config import DATABASE_PATH,os

def initialize_database(database_path: str = DATABASE_PATH) -> None:
    database_dir = os.path.dirname(database_path)
    if database_dir:
        os.makedirs(database_dir, exist_ok=True)

    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                id_dispositivo TEXT,
                dispositivo TEXT,
                id_sensor INTEGER,
                type TEXT,
                data REAL,
                unit TEXT,
                timestamp TEXT
            )
            """
        )
        conn.commit()

def save_reading(reading: Sensor, database_path: str = DATABASE_PATH) -> None:
    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            INSERT INTO sensor_data (id_dispositivo, dispositivo, id_sensor, type, data, unit, timestamp)
            VALUES (?, ?, ?, ?, ?, ?, ?)
            """,
            (reading.id_dispositivo,reading.dispositivo,reading.id_sensor, reading.type, reading.data, reading.unit, reading.timestamp),
        )
        conn.commit()

def parse_json_payload(payload: str) -> Sensor | None:
    try:
        data: dict[str, Any] = json.loads(payload)
    except json.JSONDecodeError:
        return None
    if data is None:
        return None
    try:
        return Sensor(
            id_dispositivo   =  str(data.get("id_dispositivo")),
            dispositivo      =  str(data.get("dispositivo")),
            id_sensor        =  int(data.get("id_sensor")),
            type             =  str(data.get("type")),
            data             =  float(data.get("value")),
            unit             =  str(data.get("unit")),
            timestamp        =  str(data.get("timestamp")),
        )
    except (TypeError, ValueError):
        return None


    

