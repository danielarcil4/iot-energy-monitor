import sqlite3
from typing import Any
from backend.config import DATABASE_PATH

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
                "id_sensor": measurement[3],
                "type": measurement[4],
                "data": measurement[5],
                "unit": measurement[6],
                "timestamp": measurement[7],
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
            "id_sensor": latest_measurement[3],
            "type": latest_measurement[4],
            "data": latest_measurement[5],
            "unit": latest_measurement[6],
            "timestamp": latest_measurement[7],
        }