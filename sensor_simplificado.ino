/*
 * Versión Simplificada - Sensor de Dióxido de Nitrógeno
 * Código básico para pruebas iniciales
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Configuración de pines
#define LED_PIN 10
#define BUZZER_PIN 9
#define MQ135_ANALOG A0

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
int sensorValue = 0;
int contaminationPercentage = 0;

void setup() {
  Serial.begin(9600);
  
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
  lcd.print("Sensor NO2");
  lcd.setCursor(0, 1);
  lcd.print("Listo!");
  delay(2000);
  
  Serial.println("Sensor iniciado");
}

void loop() {
  // Leer sensor
  sensorValue = analogRead(MQ135_ANALOG);
  
  // Calcular porcentaje (0-100%)
  contaminationPercentage = map(sensorValue, 0, 1023, 0, 100);
  
  // Mostrar en LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Contaminacion:");
  lcd.setCursor(0, 1);
  lcd.print(contaminationPercentage);
  lcd.print("%");
  
  // Controlar alarmas
  if (contaminationPercentage > 50) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.setCursor(8, 1);
    lcd.print("GAS!");
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  // Imprimir en Serial
  Serial.print("Valor: ");
  Serial.print(sensorValue);
  Serial.print(" | Contaminacion: ");
  Serial.print(contaminationPercentage);
  Serial.println("%");
  
  delay(1000);
} 