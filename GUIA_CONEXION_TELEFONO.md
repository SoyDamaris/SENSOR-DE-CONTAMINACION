# 📱 Guía para Conectar Sensor NO2 al Teléfono Móvil

## 🎯 **Opciones Disponibles**

### **Opción 1: Bluetooth (Más Fácil y Económica)**
**Componentes necesarios:**
- Módulo Bluetooth HC-05 o HC-06 (~$5-10)
- Arduino UNO
- Sensor MQ-135
- LCD I2C
- Buzzer y LED

### **Opción 2: WiFi con ESP8266 (Más Avanzada)**
**Componentes necesarios:**
- ESP8266 NodeMCU (~$8-15)
- Sensor MQ-135
- Buzzer y LED

---

## 🔵 **OPCIÓN 1: CONEXIÓN BLUETOOTH**

### **Paso 1: Conexiones del Hardware**

#### **Módulo Bluetooth HC-05/HC-06:**
```
VCC → 5V del Arduino
GND → GND del Arduino
TX  → Pin 2 del Arduino (BT_RX)
RX  → Pin 3 del Arduino (BT_TX)
```

#### **Conexiones Completas:**
```
Sensor MQ-135:
- VCC → 5V
- GND → GND
- A0  → A0
- D0  → Pin 8

LCD I2C:
- VCC → 5V
- GND → GND
- SDA → A4
- SCL → A5

Buzzer:
- (+) → Pin 9
- (-) → GND

LED:
- (+) → Resistencia 180Ω → Pin 10
- (-) → GND

Bluetooth:
- VCC → 5V
- GND → GND
- TX  → Pin 2
- RX  → Pin 3
```

### **Paso 2: Cargar el Código**

1. **Abrir Arduino IDE**
2. **Instalar librerías:**
   - `LiquidCrystal_I2C` (Frank de Brabander)
   - `SoftwareSerial` (incluida con Arduino)

3. **Cargar el archivo `sensor_bluetooth.ino`**

### **Paso 3: Aplicaciones para el Teléfono**

#### **Para Android:**
- **"Serial Bluetooth Terminal"** (Gratis)
- **"Bluetooth Terminal"** (Gratis)
- **"Arduino Bluetooth Controller"** (Gratis)

#### **Para iPhone:**
- **"Serial"** (Gratis)
- **"Bluetooth Terminal"** (De pago)

### **Paso 4: Configurar la Aplicación**

1. **Conectar Bluetooth:**
   - Activar Bluetooth en el teléfono
   - Buscar dispositivo "HC-05" o "HC-06"
   - Contraseña por defecto: "1234" o "0000"

2. **Configurar la app:**
   - Velocidad: 9600 baudios
   - Formato: UTF-8
   - Final de línea: CR+LF

3. **Ver datos:**
   - Los datos se envían en formato JSON
   - Se actualizan cada 2 segundos
   - Incluye porcentaje, PPM y estado de alarma

---

## 🌐 **OPCIÓN 2: CONEXIÓN WiFi**

### **Paso 1: Cambiar Hardware**

**Reemplazar Arduino UNO por ESP8266 NodeMCU:**

```
Sensor MQ-135:
- VCC → 3.3V del ESP8266
- GND → GND del ESP8266
- A0  → A0 del ESP8266

Buzzer:
- (+) → Pin D2 (GPIO4)
- (-) → GND

LED:
- (+) → Resistencia 180Ω → Pin D4 (GPIO2)
- (-) → GND
```

### **Paso 2: Configurar WiFi**

1. **Editar el código `sensor_wifi.ino`:**
```cpp
const char* ssid = "TU_WIFI_NOMBRE";
const char* password = "TU_WIFI_PASSWORD";
```

2. **Instalar librerías en Arduino IDE:**
   - `ESP8266WiFi`
   - `ESP8266WebServer`
   - `ArduinoJson`

### **Paso 3: Acceder desde el Teléfono**

1. **Conectar ESP8266 a la red WiFi**
2. **Ver la IP en el Monitor Serial**
3. **Abrir navegador en el teléfono**
4. **Ir a la IP del ESP8266** (ej: 192.168.1.100)

### **Paso 4: Interfaz Web**

La interfaz web incluye:
- ✅ **Datos en tiempo real**
- ✅ **Gráfico de contaminación**
- ✅ **Control de alarmas**
- ✅ **Diseño responsive para móvil**
- ✅ **Colores según nivel de riesgo**

---

## 📊 **APLICACIONES RECOMENDADAS**

### **Para Bluetooth:**

#### **Android - Serial Bluetooth Terminal:**
1. Descargar de Google Play
2. Conectar al módulo Bluetooth
3. Configurar velocidad 9600
4. Ver datos JSON en tiempo real

#### **iPhone - Serial:**
1. Descargar de App Store
2. Conectar vía Bluetooth
3. Configurar parámetros
4. Monitorear datos

### **Para WiFi:**

#### **Navegador Web:**
1. Abrir Chrome/Safari en el teléfono
2. Ir a la IP del ESP8266
3. Ver interfaz web completa
4. Guardar como aplicación web

---

## 🔧 **CONFIGURACIÓN AVANZADA**

### **Personalizar Umbrales:**
```cpp
// En el código, cambiar estos valores:
#define THRESHOLD_LOW 200
#define THRESHOLD_MEDIUM 400
#define THRESHOLD_HIGH 600
```

### **Calibrar Sensor:**
1. **Aire limpio:** Anotar valor base
2. **Gas conocido:** Anotar valor de referencia
3. **Ajustar fórmula** en `calculatePPM()`

### **Guardar Datos:**
```cpp
// Agregar al código para guardar en SD card:
#include <SD.h>
// ... código para guardar datos
```

---

## 📱 **APLICACIONES ESPECIALIZADAS**

### **Crear App Personalizada:**

#### **Con MIT App Inventor:**
1. Crear app con interfaz gráfica
2. Conectar vía Bluetooth
3. Mostrar datos con gráficos
4. Guardar historial

#### **Con Blynk:**
1. Crear proyecto en Blynk
2. Conectar ESP8266
3. Mostrar datos en widgets
4. Enviar notificaciones

---

## 🚨 **SOLUCIÓN DE PROBLEMAS**

### **Bluetooth no conecta:**
- Verificar contraseña (1234/0000)
- Reiniciar módulo Bluetooth
- Verificar conexiones TX/RX

### **WiFi no conecta:**
- Verificar nombre y contraseña WiFi
- Verificar que ESP8266 esté bien conectado
- Revisar Monitor Serial para errores

### **Datos no se ven:**
- Verificar velocidad de comunicación
- Comprobar formato de datos
- Revisar conexiones de pines

---

## 💡 **RECOMENDACIONES**

### **Para Principiantes:**
- **Usar Opción 1 (Bluetooth)**
- Comenzar con aplicación "Serial Bluetooth Terminal"
- Probar con datos básicos primero

### **Para Avanzados:**
- **Usar Opción 2 (WiFi)**
- Crear interfaz web personalizada
- Integrar con servicios en la nube

### **Para Producción:**
- Agregar batería de respaldo
- Implementar guardado de datos
- Crear alertas automáticas

---

## 📞 **SOPORTE**

Si tienes problemas:
1. Verificar todas las conexiones
2. Comprobar librerías instaladas
3. Revisar Monitor Serial para errores
4. Probar con código simplificado primero 