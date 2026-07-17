import sqlite3
from typing import Any
from backend.config import DATABASE_PATH

# Funcion para obtener los datos de un dispositivo
def get_data_device(database_path: str = DATABASE_PATH) -> list[dict[str, Any]] | None:
    with sqlite3.connect(database_path) as conn:
        cursor = conn.execute(
            """
            SELECT * FROM identity_data 
            ORDER BY timestamp DESC
            LIMIT 10
            """
        )
       
        devices = cursor.fetchall()

        return [
            {
                "id_dispositivo": device[1],
                "dispositivo": device[2],
            }
            for device in devices
        ]
