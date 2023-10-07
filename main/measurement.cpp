#include "Measurement.h"
#include <Arduino.h>

const int luminosityPin = A0;    // Déclaration de la broche à l'intérieur du fichier .cpp pour éviter les conflits
const int soilMoisturePin = A1;  // Déclaration de la broche à l'intérieur du fichier .cpp pour éviter les conflits

Measurement::Measurement() {
    // Initialisation, si nécessaire
    pinMode(soilMoisturePin, INPUT);
    pinMode(luminosityPin, INPUT);
}

float Measurement::getLuminosity() {
    delay(1000);
    int analogValue = analogRead(luminosityPin);
    float percentage = (analogValue / 1023.0) * 100; // Convertir la valeur analogique en pourcentage (0-100%)
    return percentage;
}

float Measurement::getSoilMoisture() {
    delay(1000);
    int analogValue = analogRead(soilMoisturePin);
    float percentage = (analogValue / 1023.0) * 100; 
    return percentage;
}

