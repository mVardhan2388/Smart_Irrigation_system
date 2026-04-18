#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

// -------------------- WIFI --------------------
const char* ssid = "Neo_Mallena_7";
const char* password = "947606139";

// -------------------- THINGSPEAK --------------------
String apiKey = "UDIWCD1PDO7G04M9";   // ESP32 Channel Write API Key
const char* server = "http://api.thingspeak.com/update";

// -------------------- DHT SENSOR --------------------
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -------------------- SOIL SENSOR --------------------
#define SOIL_PIN 34   // Analog pin
#define RELAY_PIN 5
// -------------------- VARIABLES --------------------
float temperature;
float humidity;
int soilValue;

float getRainProbability() {
  HTTPClient http;

  String url = "http://api.thingspeak.com/channels/3319454/fields/4/last.txt";

  http.begin(url);
  int httpCode = http.GET();

  float rain = 0;

  if (httpCode > 0) {
    String payload = http.getString();
    rain = payload.toFloat();
    Serial.print("Rain Probability: ");
    Serial.println(rain);
  } else {
    Serial.println("Error fetching rain data");
  }

  http.end();
  return rain;
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("\nConnected!");
}

// -------------------- LOOP --------------------
void loop() {

  // -------------------- WIFI CHECK --------------------
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting WiFi...");
    WiFi.begin(ssid, password);
    delay(5000);
    return;
  }

  // -------------------- READ SENSORS --------------------
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  soilValue = analogRead(SOIL_PIN);

  // Check DHT failure
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("------ Sensor Data ------");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Soil: "); Serial.println(soilValue);

  // -------------------- GET RAIN PROBABILITY --------------------
  float rainProb = getRainProbability();

  // -------------------- DECISION LOGIC --------------------
  int soilThreshold = 2000;  // tune this

  Serial.println("------ Decision ------");

  if (soilValue > soilThreshold && rainProb < 50) {
    digitalWrite(RELAY_PIN, HIGH);  // Pump ON
    Serial.println("Pump ON (Dry soil + No rain)");
  } 
  else {
    digitalWrite(RELAY_PIN, LOW);   // Pump OFF
    Serial.println("Pump OFF (Enough moisture OR Rain expected)");
  }

  // -------------------- SEND DATA TO THINGSPEAK --------------------
  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=" + apiKey;
  url += "&field1=" + String(temperature);
  url += "&field2=" + String(humidity);
  url += "&field3=" + String(soilValue);

  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Error sending data");
  }

  http.end();

  Serial.println("-----------------------------\n");

  // ThingSpeak delay
  delay(20000);
}