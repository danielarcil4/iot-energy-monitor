from fastapi import APIRouter
from backend.database.measurments import get_measurements

router = APIRouter()

@router.get("/devices/average")
def get_average():
    measurements = get_measurements()
    if not measurements:
        return {"message": "No measurements found"}
    # Extraemos solo los valores numéricos
    valores = [int(measurement.get("data")) for measurement in measurements]
    
    # Calculamos el promedio
    promedio = sum(valores) / len(valores)
    return {"message": f"Average: {promedio}"}
