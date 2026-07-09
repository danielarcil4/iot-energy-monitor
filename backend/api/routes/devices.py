from fastapi import APIRouter
from backend.database.data_device import get_data_device

router = APIRouter()

@router.get("/devices")
def read_devices():
    data_device = get_data_device();
    # Verify life of devices based on publiher
    return {"message": data_device}