#Conexión SQLite.
import sqlite3
from backend.models.device import IdentityDevice
from backend.config import DATABASE_PATH,os

def initialize_database(database_path: str = DATABASE_PATH) -> None:
    database_dir = os.path.dirname(database_path)
    if database_dir:
        os.makedirs(database_dir, exist_ok=True)
    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS identity_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                id_dispositivo INTEGER,
                dispositivo TEXT
            )
            """
        )
        conn.commit()

def save_reading(reading: IdentityDevice, database_path: str = DATABASE_PATH) -> None:
    with sqlite3.connect(database_path) as conn:
        conn.execute(
            """
            INSERT INTO sensor_data (id_dispositivo, dispositivo)
            VALUES (?, ?)
            """,
            (reading.id_dispositivo, reading.dispositivo),
        )
        conn.commit()




    

