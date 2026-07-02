#Conexión SQLite.
import json
import sqlite3
from typing import Any
from models import TemperatureSensor
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
                temperature REAL,
                humidity REAL,
                timestamp TEXT
            )
            """
        )
        conn.commit()

def save_reading(reading: TemperatureSensor, database_path: str = DATABASE_PATH) -> None:
    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            INSERT INTO sensor_data (temperature, timestamp)
            VALUES (?, ?)
            """,
            (reading.temperature, reading.timestamp),
        )
        conn.commit()

def parse_json_payload(payload: str) -> TemperatureSensor | None:
    try:
        data: dict[str, Any] = json.loads(payload)
    except json.JSONDecodeError:
        return None
    print(f"Datos recibidos: {data}")
    temperature = data.get("temperature", data.get("temperatura"))
    print(f"Temperatura extraida: {data['temperature']}")
    if temperature is None:
        return None

    try:
        return TemperatureSensor(
            temperature=float(temperature),
            timestamp=str(data.get("timestamp")),
        )
    except (TypeError, ValueError):
        return None

