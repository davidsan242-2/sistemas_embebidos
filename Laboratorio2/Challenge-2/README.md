# 1. Challenge 2 - ESP32 MQTT-Based IoT System

Design and implement a system where the ESP32 communicates using the MQTT protocol. The system must:

**Functional Requirements**

**1. Telemetry Publishing**
- Publish temperature and humidity readings periodically (topic naming must follow good structure, e.g., esp32/sensors/dht22).

**2. Data Logging or Visualization**
- Use an MQTT dashboard to show sensor values over time.

**3. Setpoint Subscription**
- Subscribe to a topic (e.g., esp32/control/rpm) for motor RPM setpoint.
- Display the received setpoint via Serial Monitor.

**4. Broker Connectivity**
- Use a cloud broker (e.g., HiveMQ Cloud).
- Document your topic structure and message format.

**5. Additional Notes**
* Motor hardware is not required.
* Ensure QoS selection is justified.
* All messages must follow a clean JSON structure.

## Metodología de diseño e implementación de sistemas embebidos
### 1. Análisis de requerimientos
#### Requerimientos funcionales

- FR-01: El sistema 
- FR-02: Al presionar el botón por primera vez el sistema debe encender los LEDs en secuencia a intervalos de 1s, solamente puede haber un LED encendido a la vez
- FR-03: Al presionar el botón por segunda vez el sistema debe quedar en pausa. El LED que estaba encendido cuando se presionó el botón debe quedar encendido.
- FR-04: Al presionar el botón por tercera vez el sistema debe seguir encendiendo y apagando los LEDS, comenzando por donde terminó.
- FR-05: El botón debe implementar algún método de antirrebote para evitar inconsistencias en el comportamiento del sistema
- FR-06: El botón debe activarse con un pulso negativo

#### Requerimientos no funcionales

- NFR-01: El sistema debe iniciar automaticamente al conectarse a la energía
- NFR-02: Los LEDs deben proveer el tipo de luminosidad de chorro
- NFR-03: Los LEDSs debe arreglarse en línea recta con un espaciado uniforme
- NFR-04: El color de los 4 LEDs debe ser distinto

#### Restricciones

- CON-01: Los LEDs debe conectarse a los pines GPIO y activarse con señales digitales
- CON-02: Debe implementarse una resistencia pull-Up interna para el botón
- CON-03: Los LEDs debe conectarse en serie con una resistencia para limitar la corriente
- CON-04: Se debe entregar el código del sistema en lenguaje Arduino C++ en un archivo con extensión **.ino**
- CON-05: Se dbe hacer una simulación usando [Wokwi](https://wokwi.com/)
- CON-06: Se debe hacer la implementación del sistema usando una placa de desarrollo ESP32 S3 y una protoboard

### 2. Análisis del sistema y diseño (arquitectura y componentes)

1. Placa de desarrollo ESP32 S3 DevKit C
2. Pulsador de cuatro pines
3. 4 LEDs de 3mm de diferentes colores con luminosidad a chorro
4. 4 Resistencias de 220 Ohm, 1/4 W y tolerancia de 10%.
5. 6 jumpers hembra-macho
6. 2 jumpers macho-macho
7. Protoboard

### 3. Diseño de Hardware e Integración

![3. Diseño de Harware e Integración](Challenge1-Diseño-Hardware-Sim.png)

### 4. Diseño y desarrollo del firmware

**Pseudocódigo**
```
SET ultimaActualización ← 0
SET temperatura ← 0
SET humedad ← 0
SET rpmMotor ← 0

CONECTARSE RED WIFI
CONECTARSE AL BROKER MQTT

INICIAR SENSOR DHT22

ESCUCHAR TOPICO RPM MOTOR

INICIO CICLO




    PARA I DE 1 A 4
      APAGA LED No. I
    FIN PARA

    ENCENDER LED No. ledEncendido

    SET ledEncendido ← ledEncendido + 1

    SI ledEncendido >= 5 ENTONCES
      SET ledEncendido ← 1
    FIN SI
  FIN SI

FIN CICLO
```

**Código implementación**

```
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "DHTesp.h"

const char* ssid = "univalle";
const char* password = "Univalle";

const char* mqtt_server = "7d67ac41e90249d3b6bd38d80711af39.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "david.sandoval";
const char* mqtt_password = "David4$HiveMQ";

// Definición de los canales a utilizar
const char* temperatura_topic = "esp32/sensors/dht22/temperatura";
const char* humedad_topic = "esp32/sensors/dht22/humedad";
const char* rpm_topic = "esp32/actuators/motor/rpm";

const int DHT_PIN = 42;
DHTesp dhtSensor;

String setPoint = "";

static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
char msg[50];

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el canal: ");
  Serial.println(topic);

  if (strcmp(topic, rpm_topic) == 0) {

    setPoint = "";
    for (unsigned int i = 0; i < length; i++) {
      setPoint += (char)payload[i];
    }
    Serial.print("Set Point(RPM): ");
    Serial.println(setPoint);
  } 
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(temperatura_topic);
      client.subscribe(humedad_topic);
      client.subscribe(rpm_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Challenge 2 - ESP32 MQTT-Based IoT System");
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

// Message Publishing Logic (every 5 seconds)
  long now = millis();

  if (now - lastMsg > 5000) { // Publish every 5 seconds
    lastMsg = now;
    
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    Serial.println("Temperatura: " + String(data.temperature, 2) + "°C");
    Serial.println("Humedad: " + String(data.humidity, 2) + "%");
    client.publish(temperatura_topic, String(data.temperature).c_str(), true);
    client.publish(humedad_topic, String(data.humidity).c_str(), true);
  }

}
```
### 5. Pruebas y validación

[Enlace al archivo con el código Arduino/C++](./Lab2-Challenge2/Lab2-Challenge2.ino)
