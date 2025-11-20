# 1. Challenge-3 (Simulation and Implementation)

**Weather Station**\
Design and implement an embedded system to measure temperature and humidity using two sensors: DHT22 (temperature and humidity) LM75.\
The system must read the sensors every 5 seconds using a non-blocking approach, display the temperature and humidity values locally on an LCD,
and send them through the Serial Monitor for remote observation or debugging.

## Metodología de diseño e implementación de sistemas embebidos
### 1. Análisis de requerimientos
#### Requerimientos funcionales

- FR-01: El sistema debe realizar la medición de la tempratura y humedad del ambiente
- FR-02: El sistema debe desplegar el valor de la temperatura y humedad el ambiente en una pantalla LCD de 16x2 en diferentes filas. Es decir que mostrará la temperatura en la primera fila y la humedad en la segunda fila
- FR-03: El sistema realizará la medición de temperatura y humedad cada 5 segundos
- FR-04: El sistema mostrará el valor de la temperatura y humedad en la consola serial en líneas distintas
- FR-05: El sistema debe utilizar un medidor de temperatura y humedad DHT22

#### Requerimientos no funcionales

- NFR-01: La pantalla LCD debe ser de 16x2 y tenr luz de fondo para mejor visualización

#### Restricciones

- CON-01: La pantalla LCD debe conectarse a la board ESP32 usando protocolo de comunicación I2C
- CON-02: El sensor DHT22 debe conectarse a la board ESP32 suando los pines GPIO con señales digitales
- CON-03: La pantalla LCD debe conectarse a la alimentación de 5V de la board ESP32
- CON-04: El sensor DHT22 debe conectarse a la alimentación de 3.3V de la board ESP32
- CON-05: Se debe entregar el código del sistema en lenguaje Arduino C++ en un archivo con extensión **.ino**
- CON-06: Se dbe hacer una simulación usando [Wokwi](https://wokwi.com/)
- CON-07: Se debe hacer la implementación del sistema usando una placa de desarrollo ESP32 S3 y una protoboard

### 2. Análisis del sistema y diseño (arquitectura y componentes)

1. Placa de desarrollo ESP32 S3 DevKit C
2. Sensor de temperatura y humedad DHT22
3. Pantalla LCD de 16x2 con protocolo de comunicación I2C, luz de iluminación y fondo azul
4. Protoboard

### 3. Diseño de Hardware e Integración

![3. Diseño de Harware e Integración](Challenge3-Diseño-Hardware-Sim.png)

### 4. Diseño y desarrollo del firmware

**Pseudocódigo**
```
SET ultimaActualizacion ← 0
SET intervalo ← 5000

INICIALIZAR CONSOLA SERIAL
INICIALIZAR PROTOCOLO I2C
INICIALIZAR PANTALLA LCD
INICIALIZAR SENSOR DHT22

INICIO CICLO

  SI (tiempoActual - ultimaActualización) > intervalo ENTONCES
    SET ultimaActualización ← tiempoActual

    SET temperatura ← LECTURA DE TEMPERATURA
    SET humedad ← LECTURA DE HUMEDAD

    LIMPIAR PANTALLA LCD
    DEFINIR POSICIÓN LCD (0,0)
    IMPRIMIR TEXTO LCD "T: " + temperatura + " C"
    DEFINIR POSICIÓN LCD (0,1)
    IMPRIMIR TEXTO LCD "H: " + humedad + " C"

    IMPRIMIR LINEA CONSOLA SERIAL "T: " + temperatura + " C"
    IMPRIMIR LINEA CONSOLA SERIAL "H: " + humedad + " C"
  FIN SI

FIN CICLO
```

**Código simulación**

```
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int DHT_PIN = 25;
DHTesp dhtSensor;

const int pinSDA = 21;
const int pinSCL = 22;

unsigned long ultimaActualizacion = 0;
const unsigned long intervalo = 5000;

void setup() {
  //Inicializar la comunicación por puerto serial
  Serial.begin(115200);
  //Inicializar la comunicación I2C
  Wire.begin(pinSDA, pinSCL);

  //Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Est. Tiempo");

  Serial.println("Estación de tiempo");
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - ultimaActualizacion) >= intervalo)
  {
    ultimaActualizacion = millis();
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: " + String(data.temperature, 2) + " C");

    lcd.setCursor(0, 1);
    lcd.print("H: " + String(data.humidity, 1) + " %");


    Serial.println("Temperatura: " + String(data.temperature, 2) + "°C");
    Serial.println("Humedad: " + String(data.humidity, 1) + "%");
    Serial.println("---");  
  }
}
```

**Código implementación**

```
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int DHT_PIN = 11;
DHTesp dhtSensor;

const int pinSDA = 8;
const int pinSCL = 9;

unsigned long ultimaActualizacion = 0;
const unsigned long intervalo = 5000;

void setup() {
  //Inicializar la comunicación por puerto serial
  Serial.begin(115200);
  //Inicializar la comunicación I2C
  Wire.begin(pinSDA, pinSCL);

  //Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Est. Tiempo");

  Serial.println("Estación de tiempo");
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - ultimaActualizacion) >= intervalo)
  {
    ultimaActualizacion = millis();
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: " + String(data.temperature, 2) + " C");

    lcd.setCursor(0, 1);
    lcd.print("H: " + String(data.humidity, 1) + " %");


    Serial.println("Temperatura: " + String(data.temperature, 2) + "°C");
    Serial.println("Humedad: " + String(data.humidity, 1) + "%");
    Serial.println("---");  
  }
}
```
### 5. Pruebas y validación

[Enlace al archivo con el código Arduino/C++](./Lab1-Challenge3/Lab1-Challenge3.ino)

[Enlace a la simulación](https://wokwi.com/projects/447838440179086337)

