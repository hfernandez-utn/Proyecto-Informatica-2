#include <Wire.h>
#include "DFRobot_SHT20.h"

DFRobot_SHT20 sht20;
int pinLed = 13;

class DatosClimaticos {
public:
  float humedad;
  float temperatura;
  float puntoRocio;
  float difTemperatura;

  DatosClimaticos(float humedad, float temperatura, float puntoRocio, float difTemperatura) {
    this->humedad = humedad;
    this->temperatura = temperatura;
    this->puntoRocio = puntoRocio;
    this->difTemperatura = difTemperatura;
  }

  String toString() {
    String cadenaDatos = String(temperatura, 1) + " " + String(humedad, 1) + " " + String(puntoRocio, 1) + " " + String(difTemperatura, 1);
    return cadenaDatos;
  }
};

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  Serial.println("Ejemplo SHT20!");
  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
}

void loop() {
  float humedad = sht20.readHumidity();
  float temperatura = sht20.readTemperature();

  // Calcular el punto de rocío
  float puntoRocio = temperatura - (100 - humedad) / 5.0;

  // Calcular la diferencia entre la temperatura y el punto de rocío
  float difTemperatura = temperatura - puntoRocio;

  // Enviar datos
  DatosClimaticos nuevosDatos = DatosClimaticos(humedad, temperatura, puntoRocio, difTemperatura);
  Serial.println(nuevosDatos.toString());

  // Esperar a que llegue un comando de Processing
  while (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == '1') {
      digitalWrite(pinLed, HIGH); // Encender el LED
    } else if (comando == '0') {
      digitalWrite(pinLed, LOW); // Apagar el LED
    }
  }

  delay(1000);
}

