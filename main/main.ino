#include <WiFi.h>
#include <DHT.h>
#include "Measurement.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
// Configuration du WiFi
const char* ssid = "Donne internet enculey"; // votre SSID réseau (nom)
const char* password = "mdpsimple"; // votre mot de passe réseau
WiFiServer server(80);
Measurement measurement;

void setup() {
  dht.begin();
  Serial.begin(115200);
  delay(1000);
  setupWiFi(); // Configuration et connexion au WiFi
  Serial.println("🌱 LeafLine sensor 🌱");
  Serial.println("⏳ Initializing ⏳");
  delay(2000);
  Serial.println("-----------------------------------------");
}

void loop() {
  handleClient(); // Gestion des clients WiFi
  readSensors();  // Lecture des données des capteurs
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print("\nConnected to WiFi");
  Serial.print(ssid);
  Serial.println("\n");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
}

void readSensors() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println(F("°F"));

  float moisture = measurement.getSoilMoisture();
  float luminosity = measurement.getLuminosity();
  String moistureStatus = moisture > 80 ? "🌊" : moisture > 40 ? "💧" : "🏜️";
  String lightStatus = luminosity > 80 ? "☀️" : luminosity > 40 ? "🌥️" : "🌚";
  Serial.println("Humidité du sol " + moistureStatus + ": " + String(moisture, 2) + " %");
  Serial.println("Luminosité    " + lightStatus + ": " + String(luminosity, 2) + " %");
}

void handleClient() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New Client.");
  String currentLine = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.print("GET /H turns something on.<br>");
          client.print("GET /L turns something off.<br>");
          client.println();
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }
  client.stop();
  Serial.println("Client Disconnected.");
}
