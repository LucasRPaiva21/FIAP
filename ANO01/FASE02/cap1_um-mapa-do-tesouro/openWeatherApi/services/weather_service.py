def is_going_to_rain(weather_forecast: str) -> int:
    if(weather_forecast.lower() == "rain"):
        return 1
    else:
        return 0