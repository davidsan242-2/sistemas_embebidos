// Pines de los LEDs (ajusta según tu conexión)
const int leds[4] = {47, 48, 45, 35};

// Pin del botón
const int pinBoton = 36;

// Variables de control
bool activo = false;                // Estado: encendido/apagado de la secuencia
int ledActual = 0;                  // LED actual encendido
unsigned long ultimaActualizacion = 0;        // Última actualización de LED
unsigned long tiempoBoton = 0;    // Tiempo del último cambio del botón
const unsigned long intervalo = 1000; // Intervalo entre LEDs (1 segundo)
const unsigned long tiempoRebote = 200; // Antirrebote del botón (200 ms)

// Lectura previa del botón
int ultimoEstadoBoton = HIGH;

void setup() {
  // Configuración de pines
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  pinMode(pinBoton, INPUT_PULLUP); // Botón con resistencia interna
  Serial.begin(9600);
}

void loop() {
  // --- LECTURA DEL BOTÓN CON ANTIRREBOTE ---
  int estadoBoton = digitalRead(pinBoton);

  if (estadoBoton == LOW && ultimoEstadoBoton == HIGH && (millis() - tiempoBoton > tiempoRebote)) {
    activo = !activo; // Cambia el estado (toggle)
    tiempoBoton = millis();
    Serial.print("Estado: ");
    Serial.println(activo ? "Activo" : "Parado");
  }
  ultimoEstadoBoton = estadoBoton;

  // --- CONTROL DE LA SECUENCIA ---
  if (activo && (millis() - ultimaActualizacion >= intervalo)) {
    ultimaActualizacion = millis();

    // Apagar todos los LEDs
    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], LOW);
    }

    // Encender el LED actual
    digitalWrite(leds[ledActual], HIGH);

    // Avanzar al siguiente LED
    ledActual++;
    if (ledActual >= 4) {
      ledActual = 0; // Vuelve al primero (cíclico)
    }
  }
}
