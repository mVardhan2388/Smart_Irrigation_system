import requests
import time

API_KEY = "12079324b376b2c7b67b7e320526cb28"
TS_API_KEY = "TX4VVKKL21MLDSXF"

while True:
    try:
        url = f"https://api.openweathermap.org/data/2.5/forecast?q=Guwahati&appid={API_KEY}&units=metric"
        response = requests.get(url)
        data = response.json()

        if "list" in data:
            forecast = data['list'][0]

            temp = forecast['main']['temp']
            humidity = forecast['main']['humidity']
            pressure = forecast['main']['pressure']
            pop = forecast['pop']

            rain_percent = round(pop * 100, 2)

            print(f"Temp: {temp}, Humidity: {humidity}, Pressure: {pressure}")
            print(f"Chance of rain: {rain_percent}%")
            
            ts_url = f"https://api.thingspeak.com/update?api_key={TS_API_KEY}&field1={temp}&field2={humidity}&field3={pressure}&field4={rain_percent}"
            requests.get(ts_url)

            print("Data sent to ThingSpeak!")

        else:
            print("Error from API:", data)

    except Exception as e:
        print("Error:", e)

    time.sleep(20)