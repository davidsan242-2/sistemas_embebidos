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
