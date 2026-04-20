import requests
from services.weather_service_client import get_coordinates, get_weather
from services.weather_service import is_going_to_rain

city = input("Digite a cidade para saber a previsão de chuva: ")

lat, lon = get_coordinates(city)
weather_city_data = get_weather(lat, lon)

print(weather_city_data)
is_going_to_rain = is_going_to_rain(weather_city_data)
print(is_going_to_rain)