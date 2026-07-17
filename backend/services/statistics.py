from fastapi import APIRouter
from backend.services.measurments import get_measurements

router = APIRouter()

@router.get("/measurements/average")
def get_average():
    measurements = get_measurements()
    if not measurements:
        return {"message": "No measurements found"}
    # Extraemos solo los valores numéricos
    valores = [int(measurement.get("data")) for measurement in measurements]
    
    # Calculamos el promedio
    promedio = sum(valores) / len(valores)
    return {"message": f"Average: {promedio}"}

@router.get("/measurements/min")
def get_min():
    measurements = get_measurements()
    if not measurements:
        return {"message": "No measurements found"}
    # Extraemos solo los valores numéricos
    valores = [int(measurement.get("data")) for measurement in measurements]
    
    # Calculamos el promedio
    minimo = min(valores)
    return {"message": f"Valor mínimo: {minimo}"}

@router.get("/measurements/max")
def get_max():
    measurements = get_measurements()
    if not measurements:
        return {"message": "No measurements found"}
    # Extraemos solo los valores numéricos
    valores = [int(measurement.get("data")) for measurement in measurements]
    
    # Calculamos el promedio
    maximo = max(valores)
    return {"message": f"Valor máximo: {maximo}"}