#Conexión SQLite.
import json
import sqlite3
from typing import Any
from models import Sensor
from config import DATABASE_PATH, os

def initialize_database(database_path: str = DATABASE_PATH) -> None:
    database_dir = os.path.dirname(database_path)
    if database_dir:
        os.makedirs(database_dir, exist_ok=True)

    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                type_sensor TEXT,
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
            INSERT INTO sensor_data (type_sensor, data, unit, timestamp)
            VALUES (?, ?, ?, ?)
            """,
            (reading.type_sensor, reading.data, reading.unit, reading.timestamp),
        )
        conn.commit()

def parse_json_payload(payload: str) -> Sensor | None:
    try:
        data: dict[str, Any] = json.loads(payload)
    except json.JSONDecodeError:
        return None
    type_sensor = data.get("sensor")
    if type_sensor is None:
        return None

    try:
        return Sensor(
            type_sensor=str(data.get("sensor")),
            data=float(data.get("value")),
            unit=str(data.get("unit")),
            timestamp=str(data.get("timestamp")),
        )
    except (TypeError, ValueError):
        return None

