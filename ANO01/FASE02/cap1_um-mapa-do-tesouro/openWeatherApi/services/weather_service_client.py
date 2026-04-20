import requests
from config.constants import (
    URL_GEOCODING,
    URL_WEATHER,
    API_KEY,
    UNIT_OF_MEASURE,
    LANGUAGE
)

def get_coordinates(city):
    response = requests.get(
        URL_GEOCODING,
        params={
            "appid": API_KEY,
            "q": city
        }
    )

    if response.status_code != 200:
        raise Exception("Erro ao consultar a API de localização.")

    data_city = response.json()

    if not data_city:
        raise Exception("Cidade não encontrada. Tente novamente.")

    lat = data_city[0]["lat"]
    lon = data_city[0]["lon"]

    return lat, lon


def get_weather(lat, lon) -> str:
    response = requests.get(
        URL_WEATHER,
        params={
            "appid": API_KEY,
            "lat": lat,
            "lon": lon,
            "units": UNIT_OF_MEASURE,
            "lang": LANGUAGE
        }
    )

    if response.status_code != 200:
        raise Exception("Erro ao consultar a API de clima.")

    weather_data = response.json()

    if not weather_data:
        raise Exception("Cidade não encontrada. Tente novamente.")

    return weather_data['weather'][0]['main']