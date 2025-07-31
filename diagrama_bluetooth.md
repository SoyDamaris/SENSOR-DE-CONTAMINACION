# 🔵 Diagrama de Conexiones - Módulo Bluetooth

## 📋 **Lista de Componentes**

### **Hardware Necesario:**
- ✅ Arduino UNO
- ✅ Sensor MQ-135
- ✅ Módulo Bluetooth HC-05/HC-06
- ✅ LCD 16x2 con módulo I2C
- ✅ Buzzer 5V
- ✅ LED con resistencia 180Ω
- ✅ Cables de conexión

---

## 🔌 **Diagrama de Conexiones**

```
┌─────────────────────────────────────────────────────────────┐
│                    ARDUINO UNO                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐   │
│  │   MQ-135    │    │  BLUETOOTH  │    │    LCD      │   │
│  │   SENSOR    │    │   HC-05     │    │   I2C       │   │
│  └─────────────┘    └─────────────┘    └─────────────┘   │
│  │ VCC → 5V    │    │ VCC → 5V    │    │ VCC → 5V    │   │
│  │ GND → GND   │    │ GND → GND   │    │ GND → GND   │   │
│  │ A0  → A0    │    │ TX  → Pin 2 │    │ SDA → A4    │   │
│  │ D0  → Pin 8 │    │ RX  → Pin 3 │    │ SCL → A5    │   │
│  └─────────────┘    └─────────────┘    └─────────────┘   │
│                                                             │
│  ┌─────────────┐    ┌─────────────┐                       │
│  │   BUZZER    │    │     LED     │                       │
│  └─────────────┘    └─────────────┘                       │
│  │ (+) → Pin 9 │    │ (+) → 180Ω  │                       │
│  │ (-) → GND   │    │ 180Ω → Pin10│                       │
│  └─────────────┘    │ (-) → GND   │                       │
│                     └─────────────┘                       │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 **Conexiones Detalladas**

### **1. Sensor MQ-135:**
```
MQ-135 VCC  → Arduino 5V
MQ-135 GND  → Arduino GND
MQ-135 A0   → Arduino A0 (Analógico)
MQ-135 D0   → Arduino Pin 8 (Digital)
```

### **2. Módulo Bluetooth HC-05/HC-06:**
```
Bluetooth VCC → Arduino 5V
Bluetooth GND → Arduino GND
Bluetooth TX  → Arduino Pin 2 (BT_RX)
Bluetooth RX  → Arduino Pin 3 (BT_TX)
```

### **3. LCD con módulo I2C:**
```
LCD VCC → Arduino 5V
LCD GND → Arduino GND
LCD SDA → Arduino A4 (SDA)
LCD SCL → Arduino A5 (SCL)
```

### **4. Buzzer:**
```
Buzzer (+) → Arduino Pin 9
Buzzer (-) → Arduino GND
```

### **5. LED:**
```
LED (+) → Resistencia 180Ω → Arduino Pin 10
LED (-) → Arduino GND
```

---

## ⚠️ **Notas Importantes**

### **Orden de Conexión:**
1. **Primero:** Conectar alimentación (5V y GND)
2. **Segundo:** Conectar sensores (MQ-135, LCD)
3. **Tercero:** Conectar salidas (Buzzer, LED)
4. **Cuarto:** Conectar Bluetooth

### **Verificaciones:**
- ✅ **Polaridad del LED** (pata larga = +)
- ✅ **Resistencia del LED** (180Ω obligatoria)
- ✅ **Conexiones TX/RX** del Bluetooth
- ✅ **Dirección I2C** del LCD (0x27 por defecto)

### **Configuración Bluetooth:**
- **Nombre:** HC-05 o HC-06
- **Contraseña:** 1234 o 0000
- **Velocidad:** 9600 baudios
- **Modo:** Slave

---

## 🔧 **Configuración del Código**

### **Pines Definidos:**
```cpp
#define LED_PIN 10          // LED
#define BUZZER_PIN 9        // Buzzer
#define MQ135_ANALOG A0     // Sensor analógico
#define BT_RX 2             // Bluetooth RX
#define BT_TX 3             // Bluetooth TX
```

### **Librerías Necesarias:**
```cpp
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
```

---

## 📱 **Prueba de Conexión**

### **Paso 1: Cargar Código**
1. Abrir Arduino IDE
2. Cargar `sensor_bluetooth.ino`
3. Compilar y subir

### **Paso 2: Verificar LCD**
- Debe mostrar "Sensor NO2 BT"
- Luego "Conectando..."

### **Paso 3: Conectar Bluetooth**
1. Activar Bluetooth en teléfono
2. Buscar dispositivo "HC-05"
3. Conectar con contraseña "1234"

### **Paso 4: Ver Datos**
- Abrir app "Serial Bluetooth Terminal"
- Configurar velocidad 9600
- Ver datos JSON en tiempo real

---

## 🚨 **Solución de Problemas**

### **LCD no muestra nada:**
- Verificar conexiones I2C (SDA/SCL)
- Comprobar alimentación 5V
- Revisar dirección I2C

### **Bluetooth no conecta:**
- Verificar contraseña (1234/0000)
- Comprobar conexiones TX/RX
- Reiniciar módulo Bluetooth

### **Sensor no responde:**
- Verificar alimentación 5V
- Comprobar conexión A0
- Revisar calibración

### **Alarmas no funcionan:**
- Verificar pines 9 y 10
- Comprobar polaridad del LED
- Revisar resistencia 180Ω 