#include <Wire.h>
#include "DFRobot_SHT20.h"

DFRobot_SHT20 sht20;
int ledPin = 13;

class WeatherData {
public:
  float humidity;
  float temperature;
  float dewPoint;
  float tempDifference;

  WeatherData(float humidity, float temperature, float dewPoint, float tempDifference) {
    this->humidity = humidity;
    this->temperature = temperature;
    this->dewPoint = dewPoint;
    this->tempDifference = tempDifference;
  }

  String toString() {
    String dataString = String(temperature, 1) + " " + String(humidity, 1) + " " + String(dewPoint, 1) + " " + String(tempDifference, 1);
    return dataString;
  }
};

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Serial.println("SHT20 Example!");
  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
}

void loop() {
  float humd = sht20.readHumidity();
  float temp = sht20.readTemperature();

  // Calcular el punto de rocío
  float dewPoint = temp - (100 - humd) / 5.0;

  // Calcular la diferencia entre la temperatura y el punto de rocío
  float tempDif = temp - dewPoint;

  // Enviar datos
  WeatherData newData = WeatherData(humd, temp, dewPoint, tempDif);
  Serial.println(newData.toString());

  // Esperar a que llegue un comando de Processing
  while (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      digitalWrite(ledPin, HIGH); // Encender el LED
    } else if (command == '0') {
      digitalWrite(ledPin, LOW); // Apagar el LED
    }
  }

  delay(1000);
}
