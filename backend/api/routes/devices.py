from fastapi import APIRouter

router = APIRouter()

@router.get("/devices")
def read_devices():
    # Verify life of devices based on publiher
    return {"message": "List of devices"}