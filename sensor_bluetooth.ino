/*
 * Sensor de Dióxido de Nitrógeno con Bluetooth
 * Envía datos a teléfono móvil vía Bluetooth
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Configuración de pines
#define LED_PIN 10
#define BUZZER_PIN 9
#define MQ135_ANALOG A0

// Configuración Bluetooth (HC-05/HC-06)
#define BT_RX 2  // Conectar TX del módulo Bluetooth
#define BT_TX 3  // Conectar RX del módulo Bluetooth

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Bluetooth
SoftwareSerial bluetooth(BT_RX, BT_TX);

// Variables
int sensorValue = 0;
int contaminationPercentage = 0;
float ppm = 0;
unsigned long lastDataSend = 0;
const unsigned long dataInterval = 2000; // Enviar datos cada 2 segundos

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600); // Velocidad del módulo Bluetooth
  
  // Configurar pines
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Inicializar LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Mensaje de inicio
  lcd.setCursor(0, 0);
  lcd.print("Sensor NO2 BT");
  lcd.setCursor(0, 1);
  lcd.print("Conectando...");
  
  Serial.println("Sensor Bluetooth iniciado");
  bluetooth.println("Sensor NO2 iniciado");
  
  delay(2000);
}

void loop() {
  // Leer sensor
  sensorValue = analogRead(MQ135_ANALOG);
  
  // Calcular valores
  contaminationPercentage = map(sensorValue, 0, 1023, 0, 100);
  ppm = calculatePPM(sensorValue);
  
  // Mostrar en LCD
  displayData();
  
  // Controlar alarmas
  controlAlarms();
  
  // Enviar datos por Bluetooth
  if (millis() - lastDataSend > dataInterval) {
    sendDataToPhone();
    lastDataSend = millis();
  }
  
  // Verificar comandos del teléfono
  checkPhoneCommands();
  
  delay(1000);
}

float calculatePPM(int analogValue) {
  float voltage = (analogValue * 5.0) / 1024.0;
  float ppm = 0;
  
  if (voltage > 0.5) {
    ppm = pow(10, (voltage - 0.5) * 2.5);
  }
  
  return ppm;
}

void displayData() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NO2: ");
  lcd.print(contaminationPercentage);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("BT: ");
  if (bluetooth.available()) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
}

void controlAlarms() {
  if (contaminationPercentage > 70) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (contaminationPercentage > 40) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
  } else if (contaminationPercentage > 20) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void sendDataToPhone() {
  // Formato JSON para fácil lectura en el teléfono
  bluetooth.print("{");
  bluetooth.print("\"sensor\":\"NO2\",");
  bluetooth.print("\"value\":");
  bluetooth.print(sensorValue);
  bluetooth.print(",\"percentage\":");
  bluetooth.print(contaminationPercentage);
  bluetooth.print(",\"ppm\":");
  bluetooth.print(ppm, 1);
  bluetooth.print(",\"alarm\":");
  bluetooth.print(contaminationPercentage > 50 ? "true" : "false");
  bluetooth.print(",\"timestamp\":");
  bluetooth.print(millis());
  bluetooth.println("}");
  
  Serial.println("Datos enviados por Bluetooth");
}

void checkPhoneCommands() {
  if (bluetooth.available()) {
    String command = bluetooth.readString();
    command.trim();
    
    if (command == "STATUS") {
      sendDataToPhone();
    } else if (command == "ALARM_ON") {
      digitalWrite(BUZZER_PIN, HIGH);
      bluetooth.println("Alarma activada");
    } else if (command == "ALARM_OFF") {
      digitalWrite(BUZZER_PIN, LOW);
      bluetooth.println("Alarma desactivada");
    }
  }
} 