# 1. Challenge 1 - ESP32 Standalone Web Server

Design and implement a standalone HTTP web server running on the ESP32.

The system must:

**Functional Requirements**\
**1. Environmental Monitoring**
  - Read and display temperature and relative humidity using the DHT22 sensor.
  - Refresh values periodically (configurable interval, recommended 1–5 seconds).

**2. User Input Interface**
  - Provide a web interface allowing the user to set a motor speed setpoint (RPM).
  - Show the entered setpoint in the Serial Monitor in real time.
  - Motor hardware is not required—focus on interface and data flow.

**3. Web UI Requirements**
- The server must be mobile-responsive and accessible from any device on the same network.
- UI must clearly display:
  * Sensor readings
  * Input field for RPM setpoint
  * Timestamp of last update

## Metodología de diseño e implementación de sistemas embebidos
### 1. Análisis de requerimientos
#### Requerimientos funcionales

- FR-01: El sistema percibe la humedad y la temperatura mediante el sensor DHT22
- FR-02: El sistema muestra la humedad y la temperatura por medio de una interfaz Web
- FR-03: El sistema se recarga periodicamente en un intervalo que se puede determinar en un campo de texto numérico
- FR-04: El sistema permite fijar el valor del Set Point para determinar las RPM de un motor mediante un campo de texto numérico y un botón

#### Requerimientos no funcionales

- NFR-01: El sistema debe conectarse a la Red Wifi
- NFR-02: El sistema proveer una interfaz Web para interactuar

#### Restricciones

- CON-01: Debe usarse el sensor de temperatura y humedad DHT22 para monitorear el clima
- CON-02: Se debe entregar el código del sistema en lenguaje Arduino C++ en un archivo con extensión **.ino**
- CON-03: Se debe hacer la implementación del sistema usando una placa de desarrollo ESP32 S3 y una protoboard

### 2. Análisis del sistema y diseño (arquitectura y componentes)

1. Placa de desarrollo ESP32 S3 DevKit C
2. Sensor de temperatura DHT22
5. 6 jumpers hembra-macho
6. 2 jumpers macho-macho
7. Protoboard

### 3. Diseño de Hardware e Integración

![3. Diseño de Harware e Integración](Challenge1-Diseño-Hardware-Sim.png)

### 4. Diseño y desarrollo del firmware

**Pseudocódigo**
```

SET intervalo ← 0
SET setPoint ← 0
SET temperatura ← 0
SET humedad ← 0

INICIAR CONSOLA SERIAL
INICIAR SENSOR DHT22
INICIAR SERVIDOR WEB
CONECTAR A RED WIFI

INICIO CICLO

  SI (estado = Ejecucion) & (tiempoActual - ultimaActualización) > intervaloLEDS ENTONCES
    SET ultimaActualización ← tiempoActual

    SI HAY UNA PETICIÓN DE UN CLIENTE
      ESCOJA PETICIÓN
        CASO INTERVALO
          SET intervalo ← OBTENER INTERVALO DESDE LA URL
        CASO RPM
          SET setPoint ← OBTENER RPM DESDE LA URL
      FIN ESCOJA

      SET temperatura ← Obtener temperatura desde el sensor DHT22
      SET humedad ← Obtener humedad desde el sensor DHT22

      DEVOLVER PÁGINA WEB CON VALORES ACTUALIZADOS

      IMPRIMIR RPM
      IMPRIMIR INTERVALO
    FIN SI
  FIN SI

FIN CICLO
```

**Código implementación**

```
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

#include "DHTesp.h"

// Replace with your network credentials
const char* ssid = "univalle";
const char* password = "Univalle";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

const int DHT_PIN = 42;
DHTesp dhtSensor;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

String intervalo = "10";
String rpm = "10";

void setup() {
  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

String getParam(String header, String param) {
  int start = header.indexOf(param + "=");
  if (start == -1) return "";  // Parameter not found
  
  start += param.length() + 1; // Move to the value
  int end = header.indexOf("&", start);
  int end2 = header.indexOf(" ", start);

  if (end == -1 || end > end2) end = end2;

  return header.substring(start, end);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
     
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();

      if (client.available()) {             // if there's bytes to read from the client,     
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                
            if (header.indexOf("GET /clima") >= 0) {
              intervalo = getParam(header, "intervalo");
            } else if (header.indexOf("GET /motor") >= 0)
            {
              rpm = getParam(header, "rpm");
            }

            TempAndHumidity  data = dhtSensor.getTempAndHumidity();

            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"es\">");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<title>Challenge 1 - ESP32 Standalone Web Server</title>");
            client.println("<style>");
            client.println("body {");
            client.println("font-family: Arial, sans-serif;");
            client.println("background-color: #f2f2f2;");
            client.println("text-align: center;");
            client.println("padding: 20px;");
            client.println("}");
            client.println("h1 {");
            client.println("color: #333;");
            client.println("}");
            client.println(".section {");
            client.println("background-color: #fff;");
            client.println("border-radius: 10px;");
            client.println("padding: 20px;");
            client.println("margin: 20px auto;");
            client.println("width: 300px;");
            client.println("box-shadow: 0px 0px 10px rgba(0,0,0,0.1);");
            client.println("}");
            client.println("label {");
            client.println("font-weight: bold;");
            client.println("display: block;");
            client.println("margin-top: 10px;");
            client.println("}");
            client.println("input[type=number] {");
            client.println("width: 100px;");
            client.println("padding: 5px;");
            client.println("font-size: 16px;");
            client.println("margin-top: 5px;");
            client.println("}");
            client.println("button {");
            client.println("margin-top: 15px;");
            client.println("padding: 10px 20px;");
            client.println("font-size: 16px;");
            client.println("background-color: #0078ff;");
            client.println("color: white;");
            client.println("border: none;");
            client.println("border-radius: 5px;");
            client.println("cursor: pointer;");
            client.println("}");
            client.println("button:hover {");
            client.println("background-color: #005bb5;");
            client.println("}");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>Challenge 1 - ESP32 Standalone Web Server</h1>");
            client.println("<div class=\"section\">");
            client.println("<h2>Sensor Data</h2>");
            client.println("<label>Temperatura: <span id=\"temperatura\">"+String(data.temperature, 2)+"</span> °C</label>");
            client.println("<label>Humedad: <span id=\"humedad\">"+String(data.humidity, 2)+"</span> %</label>");
            client.println("<label>Intervalo (0-60 segundos):</label>");
            client.println("<input id=\"intervalo\" type=\"number\" min=\"5\" max=\"60\" step=\"1\" value=\""+intervalo+"\">");
            client.println("</div>");
            client.println("<div class=\"section\">");
            client.println("<h2>Motor Control</h2>");
            client.println("<label>Velocidad Actual (RPM): <span id=\"rpm_actual\">"+rpm+"</span></label>");
            client.println("<label>RPM a enviar (0-150):</label>");
            client.println("<input id=\"rpm_input\" type=\"number\" min=\"0\" max=\"150\" step=\"1\" value=\""+rpm+"\">");
            client.println("<button id=\"send_rpm\">Enviar RPM</button>");
            client.println("</div>");
            client.println("<script>");
            client.println("window.addEventListener(\"DOMContentLoaded\", function() {");
            client.println("function actualizarDatos() {");
            client.println("let intervaloSec = document.getElementById(\"intervalo\").value;");
            client.println("window.location.href = \"/clima?intervalo=\"+intervaloSec;");
            client.println("}");
            client.println("let intervaloSec = document.getElementById(\"intervalo\").value;");
            client.println("let autoActualizar = setTimeout(actualizarDatos, intervaloSec * 1000);");
            client.println("document.getElementById(\"intervalo\").addEventListener(\"change\", function(){");
            client.println("clearTimeout(autoActualizar);");
            client.println("intervaloSec = parseInt(this.value);");
            client.println("autoActualizar = setTimeout(actualizarDatos, intervaloSec * 1000);");
            client.println("});");
            client.println("document.getElementById(\"send_rpm\").addEventListener(\"click\", function(){");
            client.println("let rpm = document.getElementById(\"rpm_input\").value;");
            client.println("window.location.href = \"/motor?rpm=\"+rpm;");
            client.println("});");
            client.println("});");
            client.println("</script>");
            client.println("</body>");
            client.println("</html>");

            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    Serial.println("Intervalo: "+intervalo+ " s");
    Serial.println("RPM: "+rpm);
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
```
### 5. Pruebas y validación

[Enlace al archivo con el código Arduino/C++](./Lab2-Challenge1/Lab2-Challenge1.ino)
