//Declaracion de librerias
#include <Wire.h>
#include "DFRobot_SHT20.h"


//Declara variables y objetos que se usaran
DFRobot_SHT20 sht20;
int pinLed = 13;


//Clase para almacenar datos climaticos
class DatosClimaticos {
public:
  float humedad;
  float temperatura;
  float puntoRocio;
  float difTemperatura;
//Constructor de la clase “DatosClimaticos”
  DatosClimaticos(float humedad, float temperatura, float puntoRocio, float difTemperatura) {
    this->humedad = humedad;
    this->temperatura = temperatura;
    this->puntoRocio = puntoRocio;
    this->difTemperatura = difTemperatura;
  }
//Convertir datos a cadena de texto
  String toString() {
    String cadenaDatos = String(temperatura, 1) + " " + String(humedad, 1) + " " + String(puntoRocio, 1) + " " + String(difTemperatura, 1);
    return cadenaDatos;
  }
};


void setup() {
  Serial.begin(9600);//inicia el puerto serie a 9600 baudios
  pinMode(pinLed, OUTPUT); //Define el pin 13 como salida del Led
  Serial.println("Ejemplo SHT20!");//Muestra un mensaje en el monitor serie
  sht20.initSHT20();//inicializa el sensor de temperatura y humedad
  delay(100);//Espera 100ms pra que el sensor se estabilice
  sht20.checkSHT20();//Comprueba que el sensor este funcionando correctamente
}


void loop() {
  float humedad = sht20.readHumidity();//Leer la humedad del sensor
  float temperatura = sht20.readTemperature();//Leer la temperatura del sensor


  float puntoRocio = temperatura - (100 - humedad) / 5.0;// Calcular el punto de rocío


  float difTemperatura = temperatura - puntoRocio;// Calcular la diferencia entre la temperatura y el punto de rocío


//Crea un objeto de la clase “DatosClimaticos” con los nuevos datos
  DatosClimaticos nuevosDatos = DatosClimaticos(humedad, temperatura, puntoRocio, difTemperatura);
  Serial.println(nuevosDatos.toString());//Muestra los datos en el puerto serie


  // Esperar a que llegue un comando de Processing
  while (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == '1') {
      digitalWrite(pinLed, HIGH); // Encender el LED
    } else if (comando == '0') {
      digitalWrite(pinLed, LOW); // Apagar el LED
    }
  }


  delay(1000);//Espera 1 segundo antes de repetir el bucle
}


