int pinLed1 = 8;
int pinLed2 = 9;

void setup() {
  Serial.begin(9600);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    char comando = Serial.read();
    
    if (comando == 'L') {
    
      int numLed = Serial.parseInt();
      
      int estado = Serial.parseInt();
      
      if (numLed == 1) {
        digitalWrite(pinLed1, estado);
      } else if (numLed == 2) {
        digitalWrite(pinLed2, estado);
      }
    }
  }
}
