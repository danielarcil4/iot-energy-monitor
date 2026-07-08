#Conexión SQLite.
import json
import sqlite3
from typing import Any
from .models import Sensor
from .config import DATABASE_PATH, os

def initialize_database(database_path: str = DATABASE_PATH) -> None:
    database_dir = os.path.dirname(database_path)
    if database_dir:
        os.makedirs(database_dir, exist_ok=True)

    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
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
            INSERT INTO sensor_data (id_sensor, type, data, unit, timestamp)
            VALUES (?, ?, ?, ?, ?)
            """,
            (reading.id_sensor, reading.type, reading.data, reading.unit, reading.timestamp),
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
            id_sensor=int(data.get("id_sensor")),
            type=str(data.get("type")),
            data=float(data.get("value")),
            unit=str(data.get("unit")),
            timestamp=str(data.get("timestamp")),
        )
    except (TypeError, ValueError):
        return None

def get_measurements(database_path: str = DATABASE_PATH) -> list[dict[str, Any]]:
    #return until last 100 measurements
    with sqlite3.connect(database_path) as conn:
        cursor = conn.execute(
            """
            SELECT * FROM sensor_data 
            ORDER BY timestamp DESC
            LIMIT 100
            """
        )

        measurements = cursor.fetchall()

        return [
            {
                "id_sensor": measurement[1],
                "type": measurement[2],
                "data": measurement[3],
                "unit": measurement[4],
                "timestamp": measurement[5],
            }
            for measurement in measurements
        ]

def get_latest_measurement(database_path: str = DATABASE_PATH) -> dict[str, Any] | None:
    with sqlite3.connect(database_path) as conn:
        cursor = conn.execute(
            """
            SELECT * FROM sensor_data 
            ORDER BY timestamp DESC
            LIMIT 1
            """
        )

        latest_measurement = cursor.fetchone()

        if latest_measurement is None:
            return None
        
        return {
            "id_sensor": latest_measurement[1],
            "type": latest_measurement[2],
            "data": latest_measurement[3],
            "unit": latest_measurement[4],
            "timestamp": latest_measurement[5],
        }
    
    
