/*
 * Sensor de Dióxido de Nitrógeno con Arduino UNO
 * Componentes: MQ-135, LCD I2C, Buzzer, LED
 * Autor: Mejorado para detección de contaminantes
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Configuración de pines según el diagrama
#define LED_PIN 10          // LED conectado al pin 10
#define BUZZER_PIN 9        // Buzzer conectado al pin 9
#define MQ135_DIGITAL 8     // Pin digital del sensor MQ-135
#define MQ135_ANALOG A0     // Pin analógico del sensor MQ-135

// Configuración del LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables para el sensor
int sensorValue = 0;
int sensorDigital = 0;
float ppm = 0;
int contaminationPercentage = 0;

// Umbrales de detección para NO2 (ajustar según calibración)
#define THRESHOLD_LOW 200
#define THRESHOLD_MEDIUM 400
#define THRESHOLD_HIGH 600

// Variables para el buzzer
unsigned long lastBuzzerTime = 0;
const unsigned long buzzerInterval = 500; // Intervalo del buzzer en ms

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  Serial.println("Sensor de Dióxido de Nitrógeno - Iniciando...");
  
  // Configurar pines
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ135_DIGITAL, INPUT);
  
  // Inicializar LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Mensaje de inicio
  lcd.setCursor(0, 0);
  lcd.print("Sensor NO2");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  
  Serial.println("Sistema iniciado correctamente");
}

void loop() {
  // Leer valores del sensor
  sensorValue = analogRead(MQ135_ANALOG);
  sensorDigital = digitalRead(MQ135_DIGITAL);
  
  // Calcular PPM y porcentaje de contaminación
  ppm = calculatePPM(sensorValue);
  contaminationPercentage = calculateContaminationPercentage(sensorValue);
  
  // Mostrar datos en LCD
  displayData();
  
  // Controlar alarmas
  controlAlarms();
  
  // Imprimir datos en Serial para debugging
  printSerialData();
  
  delay(1000); // Actualizar cada segundo
}

// Función para calcular PPM basado en el valor analógico
float calculatePPM(int analogValue) {
  // Fórmula aproximada para MQ-135 (ajustar según calibración)
  float voltage = (analogValue * 5.0) / 1024.0;
  float ppm = 0;
  
  if (voltage > 0.5) {
    ppm = pow(10, (voltage - 0.5) * 2.5);
  }
  
  return ppm;
}

// Función para calcular porcentaje de contaminación
int calculateContaminationPercentage(int analogValue) {
  // Mapear el valor analógico (0-1023) a porcentaje (0-100)
  int percentage = map(analogValue, 0, 1023, 0, 100);
  return constrain(percentage, 0, 100);
}

// Función para mostrar datos en LCD
void displayData() {
  lcd.clear();
  
  // Primera línea: Porcentaje de contaminación
  lcd.setCursor(0, 0);
  lcd.print("Contaminacion:");
  lcd.setCursor(0, 1);
  lcd.print(contaminationPercentage);
  lcd.print("%");
  
  // Si hay detección de gas, mostrar advertencia
  if (sensorDigital == HIGH || contaminationPercentage > 30) {
    lcd.setCursor(8, 1);
    lcd.print("GAS!");
  }
  
  delay(2000);
  
  // Segunda pantalla: PPM y estado
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PPM: ");
  lcd.print(ppm, 1);
  lcd.setCursor(0, 1);
  
  if (contaminationPercentage > 70) {
    lcd.print("ALTO RIESGO!");
  } else if (contaminationPercentage > 40) {
    lcd.print("RIESGO MEDIO");
  } else if (contaminationPercentage > 20) {
    lcd.print("BAJO RIESGO");
  } else {
    lcd.print("AIRE LIMPIO");
  }
  
  delay(2000);
}

// Función para controlar alarmas
void controlAlarms() {
  unsigned long currentTime = millis();
  
  // Detectar niveles de contaminación
  if (contaminationPercentage > 70) {
    // Alto riesgo - alarma continua
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALTA CONTAMINACION");
    lcd.setCursor(0, 1);
    lcd.print("GAS DETECTADO!");
    delay(1000);
  } else if (contaminationPercentage > 40) {
    // Riesgo medio - alarma intermitente
    if (currentTime - lastBuzzerTime > buzzerInterval) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
      lastBuzzerTime = currentTime;
    }
  } else if (contaminationPercentage > 20) {
    // Bajo riesgo - LED parpadeante
    if (currentTime - lastBuzzerTime > buzzerInterval * 2) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      digitalWrite(BUZZER_PIN, LOW);
      lastBuzzerTime = currentTime;
    }
  } else {
    // Sin riesgo - apagar alarmas
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// Función para imprimir datos en Serial
void printSerialData() {
  Serial.print("Valor Analógico: ");
  Serial.print(sensorValue);
  Serial.print(" | Digital: ");
  Serial.print(sensorDigital);
  Serial.print(" | PPM: ");
  Serial.print(ppm, 1);
  Serial.print(" | Contaminación: ");
  Serial.print(contaminationPercentage);
  Serial.println("%");
  
  if (contaminationPercentage > 30) {
    Serial.println("¡GAS DETECTADO!");
  }
} 