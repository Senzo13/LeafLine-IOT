#include <WiFi.h>
#include <DHT.h>
#include "Measurement.h"
#include <HTTPClient.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "Donne internet enculey"; // Votre SSID
const char* password = "mdpsimple";  // Votre mot de passe
WiFiServer server(80);
Measurement measurement;
HTTPClient http;

void setup() {
  dht.begin();
  Serial.begin(115200);
  delay(1000);
  setupWiFi(); 
  Serial.println("🌱 LeafLine sensor 🌱");
  Serial.println("⏳ Initializing ⏳");
  delay(2000);
  Serial.println("-----------------------------------------");
}

void loop() {
  handleClient();
  readSensors();
  delay(100); // Attendre 1 secondes entre les lectures
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(ssid);
  Serial.println();
  Serial.println("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
}

void readSensors() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h, 1);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(t, 1);
  Serial.println("°C");

  float moisture = measurement.getSoilMoisture();
  float luminosity = measurement.getLuminosity();
  String moistureStatus = moisture > 80 ? "🌊" : moisture > 40 ? "💧" : "🏜️";
  String lightStatus = luminosity > 80 ? "☀️" : luminosity > 40 ? "🌥️" : "🌚";
  Serial.println("Humidité du sol " + moistureStatus + ": " + String(moisture, 2) + " %");
  Serial.println("Luminosité    " + lightStatus + ": " + String(luminosity, 2) + " %");
  Serial.println("-----------------------------------------");

  // getData("temperatures"); // Requête GET pour tester
  // getData(""); // Requête GET pour tester
  patchData("sunshineData/65b958ccc8f6cb4a4f0f365d/add", luminosity);
  patchData("temperatures/65b958cbc8f6cb4a4f0f2c34/add", t); // Requête PATCH
  patchData("airHumidityData/65b958cac8f6cb4a4f0f0db9/add", h);
  // patchData("soilHumidityData/65b958cbc8f6cb4a4f0f17e2/add", moisture);
}

void getData(const char* path) {
  if(WiFi.status() == WL_CONNECTED) {
    String serverPath = "https://leafline.ddns.net/";
    serverPath += path;

    http.begin(serverPath);
    int httpResponseCode = http.GET();

    if(httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("GET response: ");
      Serial.println(payload);
    } else {
      Serial.print("Error on sending GET request: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void patchData(const char* path, float data) {
  if(WiFi.status() == WL_CONNECTED) {
    String serverPath = "https://leafline.ddns.net/";
    serverPath += path;

    http.begin(serverPath);
    http.addHeader("Content-Type", "application/json");

    String httpRequestData = "{\"value\":" + String(data) + "}";
    int httpResponseCode = http.PATCH(httpRequestData);

    if(httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("PATCH response: ");
      Serial.println(response);
    } else {
      Serial.print("Error on sending PATCH request: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void handleClient() {
  WiFiClient client = server.available();
  if (!client) return;

  while (client.connected()) {
    if (client.available()) {
      client.readStringUntil('\r');
      client.flush();
    }
  }
  client.stop();
}
