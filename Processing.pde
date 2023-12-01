//Importa las librerias necesarias
import processing.serial.*;
import processing.data.Table;
import processing.data.TableRow;

//Declara variables globales
Serial myPort;
Table dataTable;
int lastMinute = 0;
boolean ledEncendido = false;
Button ledButton;

void setup() {
//Configura el puerto serie
  String portName = "COM6";  
  myPort = new Serial(this, portName, 9600);

//Configuracion de la pantalla
  size(800, 600);
  textAlign(CENTER, TOP);
  textSize(14);
 
//Crea una tabla para almacenar los datos 
  dataTable = new Table();
  dataTable.addColumn("Hora");
  dataTable.addColumn("Temperatura");
  dataTable.addColumn("Humedad");
  dataTable.addColumn("Punto de Rocío");
  dataTable.addColumn("Diferencia de Temperatura");
 
//Inicializa el boton para encender led 
  ledButton = new Button(width - 150, 10, 120, 40, "Encender LED", color(255, 0, 0), color(0, 255, 0));
}

void draw() {
//Lee datos del puerto serie si hay disponibles
  while (myPort.available() > 0) {
    String data = myPort.readStringUntil('\n');
    
    if (data != null) {
//Divide los datos en valores individuales
      String[] values = split(data, ' ');
      
      if (values.length >= 4) {
//Convierte valores recibidos en variables 
        float temp = float(values[0]);
        float humd = float(values[1]);
        float dewPoint = float(values[2]);
        float tempDif = float(values[3]);
        
        int currentMinute = minute();
        
//Coloca datos en la tabla cada 1 minuto
        if (currentMinute != lastMinute) {
          TableRow newRow = dataTable.addRow();
          newRow.setInt("Hora", hour() * 3600 + minute() * 60 + second());
          newRow.setFloat("Temperatura", temp);
          newRow.setFloat("Humedad", humd);
          newRow.setFloat("Punto de Rocío", dewPoint);
          newRow.setFloat("Diferencia de Temperatura", tempDif);
          lastMinute = currentMinute;
        }
        
//Dibuja la interfaz con los datos en tiempo real
        background(240);
        fill(0);
        text("Hora: " + formatTime(hour() * 3600 + minute() * 60 + second()), width / 2, 10);
        text("Temperatura: " + String.format("%.1f", temp) + " °C", width / 2, 30);
        text("Humedad: " + String.format("%.1f", humd) + " %", width / 2, 50);
        text("Punto de Rocío: " + String.format("%.1f", dewPoint) + " °C", width / 2, 70);
        text("Diferencia de Temperatura: " + String.format("%.1f", tempDif) + " °C", width / 2, 90);
        
//Dibuja el boton de encender ldy y la tabla de datos
        ledButton.draw();
        displayTable();
      }
    }
  }
}

//Mostrar la tabla de datos almacenados
void displayTable() {
  float tableX = 30;
  float tableY = 140;
  float cellWidth = 120;
  float cellHeight = 30;
  
  fill(0);
  text("Datos almacenados:", width / 2, tableY - 20);
  text("Hora", tableX, tableY);
  text("Temperatura", tableX + cellWidth, tableY);
  text("Humedad", tableX + 2 * cellWidth, tableY);
  text("Punto de Rocío", tableX + 3 * cellWidth, tableY);
  text("Diferencia T y R", tableX + 4 * cellWidth, tableY);
  


//Recorre la tabla y muestra los datos
  for (int i = dataTable.getRowCount() - 1; i >= 0; i--) {
    TableRow row = dataTable.getRow(i);
    int totalSeconds = row.getInt("Hora");
    text(formatTime(totalSeconds), tableX, tableY + (dataTable.getRowCount() - i) * cellHeight);
    text(String.format("%.1f", row.getFloat("Temperatura")), tableX + cellWidth, tableY + (dataTable.getRowCount() - i) * cellHeight);
    text(String.format("%.1f", row.getFloat("Humedad")), tableX + 2 * cellWidth, tableY + (dataTable.getRowCount() - i) * cellHeight);
    text(String.format("%.1f", row.getFloat("Punto de Rocío")), tableX + 3 * cellWidth, tableY + (dataTable.getRowCount() - i) * cellHeight);
    text(String.format("%.1f", row.getFloat("Diferencia de Temperatura")), tableX + 4 * cellWidth, tableY + (dataTable.getRowCount() - i) * cellHeight);
  }
}

//Funcion para dar formato a la hora
String formatTime(int seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int remainingSeconds = seconds % 60;
  return String.format("%02d:%02d:%02d", hours, minutes, remainingSeconds);
}

//Clase para el boton de encendido y apagado del led
class Button {
  float x, y, width, height;
  String label;
  color defaultColor, hoverColor;
  boolean hovering = false;

//Constructor del boton de encendido y apagado del led
  Button(float x, float y, float width, float height, String label, color defaultColor, color hoverColor) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.label = label;
    this.defaultColor = defaultColor;
    this.hoverColor = hoverColor;
  }

//Funcion para dibujar el boton de encendido y apagado del led
  void draw() {
    fill(hovering ? hoverColor : defaultColor);
    rect(x, y, width, height);

    fill(255);
    textAlign(CENTER, CENTER);
    text(label, x + width / 2, y + height / 2);
  }


//Verificar si el puntero esta sobre el boton de encendido y apagado del led
  boolean isMouseOver() {
    return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
  }
}

//Funcion para gestionar el movimiento y click del mouse
void mouseMoved() {
  ledButton.hovering = ledButton.isMouseOver();
}

void mousePressed() {
  if (ledButton.isMouseOver()) {
    ledEncendido = !ledEncendido;

Envia comando a arduino para encender o apagar el led
    if (ledEncendido) {
      myPort.write('1');
      ledButton.label = "Apagar LED";
      ledButton.defaultColor = color(0, 255, 0);
    } else {
      myPort.write('0');
      ledButton.label = "Encender LED";
      ledButton.defaultColor = color(255, 0, 0);
    }
  }
}
