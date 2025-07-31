/*
 * Sensor de Dióxido de Nitrógeno con WiFi
 * Usando ESP8266 para conectar a internet
 * Acceso vía navegador web en el teléfono
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Configuración WiFi
const char* ssid = "TU_WIFI_NOMBRE";
const char* password = "TU_WIFI_PASSWORD";

// Configuración de pines
#define LED_PIN 2
#define BUZZER_PIN 4
#define MQ135_ANALOG A0

// Servidor web
ESP8266WebServer server(80);

// Variables
int sensorValue = 0;
int contaminationPercentage = 0;
float ppm = 0;
unsigned long lastDataUpdate = 0;

void setup() {
  Serial.begin(9600);
  
  // Configurar pines
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Conectar WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());
  
  // Configurar servidor web
  setupWebServer();
  server.begin();
  
  Serial.println("Servidor web iniciado");
}

void loop() {
  // Leer sensor
  sensorValue = analogRead(MQ135_ANALOG);
  
  // Calcular valores
  contaminationPercentage = map(sensorValue, 0, 1023, 0, 100);
  ppm = calculatePPM(sensorValue);
  
  // Controlar alarmas
  controlAlarms();
  
  // Manejar servidor web
  server.handleClient();
  
  delay(1000);
}

float calculatePPM(int analogValue) {
  float voltage = (analogValue * 3.3) / 1024.0; // ESP8266 usa 3.3V
  float ppm = 0;
  
  if (voltage > 0.5) {
    ppm = pow(10, (voltage - 0.5) * 2.5);
  }
  
  return ppm;
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

void setupWebServer() {
  // Página principal
  server.on("/", HTTP_GET, []() {
    String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Sensor NO2</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; text-align: center; margin: 20px; }
        .data { font-size: 24px; margin: 20px; padding: 20px; border-radius: 10px; }
        .good { background-color: #90EE90; }
        .warning { background-color: #FFD700; }
        .danger { background-color: #FF6B6B; color: white; }
        .chart { width: 100%; max-width: 400px; margin: 20px auto; }
    </style>
</head>
<body>
    <h1>🌬️ Sensor de Dióxido de Nitrógeno</h1>
    
    <div class="data" id="status">
        <h2>Estado Actual</h2>
        <p>Contaminación: <span id="percentage">--</span>%</p>
        <p>PPM: <span id="ppm">--</span></p>
        <p>Estado: <span id="status">--</span></p>
    </div>
    
    <div class="chart">
        <canvas id="chart" width="400" height="200"></canvas>
    </div>
    
    <button onclick="refreshData()">🔄 Actualizar</button>
    <button onclick="toggleAlarm()">🔊 Alarma</button>
    
    <script>
        let chartData = [];
        
        function refreshData() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('percentage').textContent = data.percentage;
                    document.getElementById('ppm').textContent = data.ppm.toFixed(1);
                    document.getElementById('status').textContent = data.status;
                    
                    // Actualizar color según nivel
                    const statusDiv = document.getElementById('status');
                    statusDiv.className = 'data ' + data.level;
                    
                    // Agregar datos al gráfico
                    chartData.push({
                        time: new Date().toLocaleTimeString(),
                        value: data.percentage
                    });
                    
                    if (chartData.length > 10) {
                        chartData.shift();
                    }
                    
                    updateChart();
                });
        }
        
        function updateChart() {
            const canvas = document.getElementById('chart');
            const ctx = canvas.getContext('2d');
            
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.strokeStyle = '#333';
            ctx.lineWidth = 2;
            
            if (chartData.length > 1) {
                ctx.beginPath();
                chartData.forEach((point, index) => {
                    const x = (index / (chartData.length - 1)) * canvas.width;
                    const y = canvas.height - (point.value / 100) * canvas.height;
                    
                    if (index === 0) {
                        ctx.moveTo(x, y);
                    } else {
                        ctx.lineTo(x, y);
                    }
                });
                ctx.stroke();
            }
        }
        
        function toggleAlarm() {
            fetch('/alarm')
                .then(response => response.text())
                .then(data => alert(data));
        }
        
        // Actualizar cada 5 segundos
        setInterval(refreshData, 5000);
        refreshData();
    </script>
</body>
</html>
    )";
    
    server.send(200, "text/html", html);
  });
  
  // API para datos JSON
  server.on("/data", HTTP_GET, []() {
    String status = "Normal";
    String level = "good";
    
    if (contaminationPercentage > 70) {
      status = "ALTO RIESGO!";
      level = "danger";
    } else if (contaminationPercentage > 40) {
      status = "Riesgo Medio";
      level = "warning";
    } else if (contaminationPercentage > 20) {
      status = "Bajo Riesgo";
      level = "warning";
    }
    
    StaticJsonDocument<200> doc;
    doc["sensor"] = "NO2";
    doc["value"] = sensorValue;
    doc["percentage"] = contaminationPercentage;
    doc["ppm"] = ppm;
    doc["status"] = status;
    doc["level"] = level;
    doc["timestamp"] = millis();
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });
  
  // Control de alarma
  server.on("/alarm", HTTP_GET, []() {
    digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
    String response = digitalRead(BUZZER_PIN) ? "Alarma activada" : "Alarma desactivada";
    server.send(200, "text/plain", response);
  });
} 