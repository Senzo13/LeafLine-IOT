#include "LiquidCrystal.h"
#include "Measurement.h"    // inclusion de la classe Measurement
#include <WiFiEsp.h>
#include <DHT.h>

// Définir le pin de données du capteur DHT11
#define DHTPIN 2
// Définir le type de capteur DHT11
#define DHTTYPE DHT11
// Initialiser le capteur DHT11
DHT dht(DHTPIN, DHTTYPE);

// #ifndef HAVE_HWSERIAL1
// #include "SoftwareSerial.h"
// SoftwareSerial Serial1(19, 18); // RX, TX
// #endif

// // Configuration du WiFi
// char ssid[] = "Livebox-CB9C";            // your network SSID (name)
// char pass[] = "ChibreMarin23";        // your network password
// int status = WL_IDLE_STATUS;     // the WiFi radio's status
// WiFiEspServer server(80);

// Vérifiez les broches !
LiquidCrystal lcd(11,10,9,8,7,6,5,4,3,2); // liaison 8 bits de données (commentée car vous avez choisi 4 bits)
// LiquidCrystal lcd(11,10,5,4,3,2); // liaison 4 bits de données

Measurement measurement;          // créer une instance de la classe Measurement

void setup() {

  Serial.begin(9600);
 // Initialiser le capteur DHT11
  dht.begin();
  //Init la wifi
  // wifiInit();

 //- - - - - - - - - - - - - - - -
  delay(1000);
  Serial.println("🌱 LeafLine sensor 🌱");
  Serial.println("⏳ Initializing ⏳");
  delay(2000);
  //- - - - - - - - - - - - - - - -
  Serial.println("-----------------------------------------");
}

// void wifiInit() {
//   // Initialize Wi-Fi module
//     Serial1.begin(115200);
//     WiFi.init(&Serial1);
//   // Start the web server on port 80
//   server.begin();
//     // Check for the presence of the shield
//     if (WiFi.status() == WL_NO_SHIELD) {
//         Serial.println("WiFi shield not present");
//         // Halt execution
//         while (true);
//     }

//     // Attempt to connect to WiFi network
//     connectToWifi();
// }

void loop() {
  float moisture = measurement.getSoilMoisture();
  float luminosity = measurement.getLuminosity();
  float temperature = dht.readTemperature();
  float airHumidity = dht.readHumidity();
  
  String moistureStatus = moisture > 80 ? "🌊" : moisture > 40 ? "💧" : "🏜️";
  String lightStatus = luminosity > 80 ? "☀️" : luminosity > 40 ? "🌥️" : "🌚";
  
  // Pour l'humidité de l'air
  String airHumidityStatus = airHumidity > 80 ? "🌧️" : airHumidity > 40 ? "💧" : "🏜️";
  
  // Pour la température
  String temperatureStatus;
  if (temperature > 30) {
    temperatureStatus = "🔥";
  } else if (temperature > 15) {
    temperatureStatus = "🌡️";
  } else {
    temperatureStatus = "❄️";
  }

  Serial.println("Soil moisture " + moistureStatus + ": " + String(moisture, 2) + " %");
  Serial.println("Luminosity    " + lightStatus + ": " + String(luminosity, 2) + " %");
  Serial.println("Air humidity  " + airHumidityStatus + ": " + String(airHumidity, 2) + " %");
  Serial.println("Temperature   " + temperatureStatus + ": " + String(temperature, 2) + " °C");

  Serial.println("-----------------------------------------");

  // handleClientRequests();
}


// void connectToWifi() {
//     while (status != WL_CONNECTED) {
//         Serial.print("Attempting to connect to WPA SSID: ");
//         Serial.println(ssid);
//         // Connect to WPA/WPA2 network
//         status = WiFi.begin(ssid, pass);
//     }
    
//     Serial.println("You're connected to the network");
//     printWifiStatus();
// }

// void handleClientRequests() {
//     // Listen for incoming clients
//     WiFiEspClient client = server.available();
//     if (client) {
//         Serial.println("New client");
//         processClientRequest(client);
//     }
// }

// void processClientRequest(WiFiEspClient& client) {
//     bool currentLineIsBlank = true;

//     while (client.connected()) {
//         if (client.available()) {
//             char c = client.read();
//             Serial.write(c);

//             if (c == '\n' && currentLineIsBlank) {
//                 Serial.println("Sending response");
//                 sendHttpResponse(client);
//                 break;
//             }

//             currentLineIsBlank = (c == '\n' || c == '\r');
//         }
//     }

//     delay(10);  // Give the web browser time to receive the data
//     client.stop();
//     Serial.println("Client disconnected");
// }

// void sendHttpResponse(WiFiEspClient& client) {
//     client.print(
//         "HTTP/1.1 200 OK\r\n"
//         "Content-Type: application/json\r\n"
//         "Connection: close\r\n"  // The connection will be closed after completion of the response
//         "\r\n"
//     );
    
//     // Assuming you want to send the same JSON data as before
//     String sensorData = "{...}";  // ... represents the JSON data
//     client.print(sensorData);
// }

// void printWifiStatus() {
//     Serial.print("SSID: ");
//     Serial.println(WiFi.SSID());

//     IPAddress ip = WiFi.localIP();
//     Serial.print("IP Address: ");
//     Serial.println(ip);

//     Serial.println();
//     Serial.print("To see this page in action, open a browser to http://");
//     Serial.println(ip);
//     Serial.println();
// }