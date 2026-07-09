from fastapi import APIRouter
from backend.database.measurments import get_measurements, get_latest_measurement
router = APIRouter()

@router.get("/measurements")
def read_measurements():
    measurements = get_measurements()
    return {"message": measurements}

@router.get("/measurements/latest")
def read_latest_measurements():
    latest_measurement = get_latest_measurement()
    return {"message": latest_measurement}