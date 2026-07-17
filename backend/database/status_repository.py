#Conexión SQLite.
import sqlite3
from backend.models.status import statusDevice
from backend.config import DATABASE_PATH,os

def initialize_database(database_path: str = DATABASE_PATH) -> None:
    database_dir = os.path.dirname(database_path)
    if database_dir:
        os.makedirs(database_dir, exist_ok=True)
    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS status_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                uptime INTEGER,
                wifi_rssi INTEGER,
                free_heap INTEGER,
                message_sent INTEGER,
                online BOOLEAN,
            )
            """
        )
        conn.commit()

def save_reading(reading: statusDevice, database_path: str = DATABASE_PATH) -> None:
    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            INSERT INTO sensor_data (uptime, wifi_rssi, free_heap, message_sent, online)
            VALUES (?, ?, ?, ?, ?)
            """,
            (reading.uptime, reading.wifi_rssi, reading.free_heap, reading.messages_sent, reading.online),
        )
        conn.commit()




    

