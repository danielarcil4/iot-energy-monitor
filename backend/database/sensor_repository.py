#Conexión SQLite.
import sqlite3
from backend.models.sensor import Sensor
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
                type TEXT,
                value REAL,
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
            INSERT INTO sensor_data (type, value, unit, timestamp)
            VALUES (?, ?, ?, ?)
            """,
            (reading.type, reading.value, reading.unit, reading.timestamp),
        )
        conn.commit()



    

