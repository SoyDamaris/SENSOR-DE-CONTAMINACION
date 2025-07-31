# Sensor de Dióxido de Nitrógeno con Arduino UNO

## Descripción del Proyecto
Este proyecto implementa un sensor de dióxido de nitrógeno (NO2) utilizando un sensor MQ-135 para detectar contaminantes en el aire. El sistema incluye una pantalla LCD para mostrar datos en tiempo real, alarmas sonoras y visuales, y capacidad de registro de datos.

## Componentes Utilizados

### Hardware
- **Arduino UNO** - Microcontrolador principal
- **Sensor MQ-135** - Sensor de gas para detección de NO2
- **LCD 16x2 con módulo I2C** - Pantalla para mostrar datos
- **Buzzer 5V** - Alarma sonora
- **LED con resistencia 180Ω** - Indicador visual
- **Cables de conexión** - Para conectar todos los componentes

### Conexiones del Circuito

#### Sensor MQ-135
- **VCC** → **5V** del Arduino
- **GND** → **GND** del Arduino  
- **D0** → **Pin Digital 8** del Arduino
- **A0** → **Pin Analógico A0** del Arduino

#### Buzzer
- **Pin positivo** → **Pin Digital 9** del Arduino
- **Pin negativo** → **GND** del Arduino

#### LED
- **Ánodo (pata larga)** → **Resistencia 180Ω** → **Pin Digital 10** del Arduino
- **Cátodo (pata corta)** → **GND** del Arduino

#### LCD con módulo I2C
- **VCC** → **5V** del Arduino
- **GND** → **GND** del Arduino
- **SDA** → **Pin A4 (SDA)** del Arduino
- **SCL** → **Pin A5 (SCL)** del Arduino

## Características del Código

### Funcionalidades Principales
1. **Detección de Gas**: Monitoreo continuo de niveles de NO2
2. **Cálculo de PPM**: Conversión de valores analógicos a partes por millón
3. **Porcentaje de Contaminación**: Mapeo de valores a porcentajes (0-100%)
4. **Sistema de Alarmas**:
   - **Alto Riesgo (>70%)**: Alarma continua con LED y buzzer
   - **Riesgo Medio (40-70%)**: Alarma intermitente
   - **Bajo Riesgo (20-40%)**: LED parpadeante
   - **Sin Riesgo (<20%)**: Sin alarmas

### Pantalla LCD
El LCD muestra información en dos pantallas alternadas:
1. **Pantalla 1**: Porcentaje de contaminación y estado de detección
2. **Pantalla 2**: PPM y nivel de riesgo

### Salida Serial
Para debugging y registro de datos, el código incluye salida serial con:
- Valor analógico del sensor
- Estado digital del sensor
- PPM calculado
- Porcentaje de contaminación
- Alertas de detección

## Instalación y Configuración

### 1. Instalación de Librerías
En Arduino IDE, instalar las siguientes librerías:
- `LiquidCrystal_I2C` (Frank de Brabander)
- `Wire` (incluida con Arduino)

### 2. Conexión del Hardware
1. Conectar todos los componentes según el diagrama
2. Verificar que el módulo I2C esté correctamente conectado
3. Asegurar que la resistencia del LED sea de 180Ω

### 3. Carga del Código
1. Abrir Arduino IDE
2. Cargar el archivo `sensor_dioxido_nitrogeno.ino`
3. Seleccionar la placa Arduino UNO
4. Compilar y subir el código

### 4. Calibración (Opcional)
Para mejorar la precisión, ajustar los umbrales en el código:
```cpp
#define THRESHOLD_LOW 200
#define THRESHOLD_MEDIUM 400
#define THRESHOLD_HIGH 600
```

## Uso del Sistema

### Inicio
1. Conectar la alimentación al Arduino
2. El LCD mostrará "Sensor NO2 - Iniciando..."
3. El sistema comenzará a monitorear automáticamente

### Interpretación de Resultados
- **0-20%**: Aire limpio, sin alarmas
- **20-40%**: Bajo riesgo, LED parpadeante
- **40-70%**: Riesgo medio, alarma intermitente
- **70-100%**: Alto riesgo, alarma continua

### Registro de Datos
Los datos se pueden capturar desde el Monitor Serial para análisis posterior.

## Mantenimiento

### Calibración Periódica
- Realizar calibración cada 6 meses
- Usar aire limpio como referencia
- Ajustar umbrales según el entorno

### Limpieza del Sensor
- Limpiar el sensor MQ-135 periódicamente
- Evitar exposición a gases corrosivos
- Mantener en ambiente seco

## Solución de Problemas

### LCD no muestra nada
- Verificar conexiones I2C (SDA y SCL)
- Comprobar dirección I2C (0x27 por defecto)
- Revisar alimentación del módulo

### Sensor no responde
- Verificar conexiones de alimentación
- Comprobar pines de datos
- Revisar calibración del sensor

### Alarmas no funcionan
- Verificar conexiones del buzzer y LED
- Comprobar pines de salida
- Revisar valores de umbral

## Especificaciones Técnicas

- **Voltaje de operación**: 5V
- **Rango de detección**: 0-1000 ppm NO2
- **Tiempo de respuesta**: <10 segundos
- **Temperatura de operación**: -10°C a +50°C
- **Humedad de operación**: 20-90% RH

## Licencia
Este proyecto es de código abierto y está disponible para uso educativo y personal. 