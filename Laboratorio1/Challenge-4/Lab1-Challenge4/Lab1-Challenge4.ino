#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
//Pines de dirección del motor
const int dirPin1 = 15;
const int dirPin2 = 16;
const int velPin = 7;

const int pinChannelA = 9;
const int pinChannelB = 46;

const int pinSDA = 40;
const int pinSCL = 39;

const int freq = 5000;
const int resolution = 8;
int pwmValue;

//Relación de engranajes 1:30
const int pprM = 11; //Pulsos por revolución del motor
const unsigned long pprE = 330; //Pulsos por revolución del eje

volatile unsigned long contadorPulsosA;
volatile unsigned long contadorPulsosB;

float rpmCalculado;

unsigned long ultimaActualizacionEjecucion;
const long intervaloPrincipalEjecucion = 100;

unsigned long ultimaActualizacionMostrarMensaje;
const long intervaloPrincipalMensaje = 1000;

//Variables de control PID
float cv;
float cv1;

float error;
float error1;
float error2;

float kp = 1;
float ki = 1;
float kd = 0.01;
float Tm = 0.1;

float setPoint;

enum Estado {
  DETENIDO,
  MENUDESPLEGADO,
  SETPOINT,
  EJECUCION
};

enum Direccion {
  SENTIDO_HORARIO,
  SENTIDO_ANTIHORARIO
};

Direccion direccionActual;
Estado estadoActual = DETENIDO;

bool isInteger(String str) {
    if (str.length() == 0) {
        return false; // Empty string is not a valid integer
    }
    
    int startIndex = 0;
   
    // A string with only a minus sign (e.g., "-") is not a valid integer
    if (startIndex >= str.length()) {
        return false;
    }

    // Check the rest of the characters
    for (int i = startIndex; i < str.length(); i++) {
        if (!isDigit(str.charAt(i))) { // Use the built-in isDigit() function
            return false; // Found a non-digit character
        }
    }
    
    return true; // All characters are valid digits (and maybe a leading minus)
}

void encenderMotor() {
  ledcAttach(velPin, freq, resolution);
  if (direccionActual == SENTIDO_HORARIO)
  {
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, HIGH);
  } else
  {
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, LOW);    
  }
  ledcWrite(velPin, 30);
}

void apagarMotor() { 
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  ledcWrite(velPin, 0);
}

void IRAM_ATTR interrupcionCanalA() {
  contadorPulsosA++;
}

void IRAM_ATTR interrupcionCanalB() {
  contadorPulsosB++;
}

void setup() {
  Serial.begin(115200);

  //Inicializar la comunicación I2C
  Wire.begin(pinSDA, pinSCL);

  //Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Control Motor");

  //Inicializar variables
  pwmValue = 0;
  contadorPulsosA = 0;
  contadorPulsosB = 0;
  rpmCalculado = 0;
  ultimaActualizacionEjecucion = 0;
  ultimaActualizacionMostrarMensaje = 0;
  setPoint = 9;
  
  //Configurar los pines digitales de salida de la dirección del motor
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  //El motor inicia parado
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);
  digitalWrite(velPin, LOW);

  //Configurar los pines digitales de entrada del encoder del motor
  //pinMode(pinChannelA, INPUT_PULLUP);
  pinMode(pinChannelA, INPUT); 
  pinMode(pinChannelB, INPUT);
  //pinMode(pinChannelB, INPUT_PULLUP);
  attachInterrupt(pinChannelA, interrupcionCanalA, RISING);
  attachInterrupt(pinChannelB, interrupcionCanalB, RISING);

  delay(2000);
}

void loop() {
  String entradaUsuario = "";
  switch (estadoActual)
  {
    case DETENIDO:
      Serial.println();
      Serial.println("Menú del Motor con Encoder:");
      Serial.println("1. S: Definir set point de velocidad.");
      Serial.println("2. X: Detener motor.");
      estadoActual = MENUDESPLEGADO;      
    break;
    case MENUDESPLEGADO:
      if (Serial.available() > 0)
      {
        entradaUsuario = Serial.readStringUntil('\n');
        if (entradaUsuario == "S")
        {
          Serial.println("Por favor ingrese el set point de la velocidad.");
          Serial.println("El valor ingresado debe ser un número de 0 a 150");
          Serial.println("Debe anteceder el valor el signo de velocidad:");
          Serial.println("+ (rotación en sentido horario)");
          Serial.println("- (rotación en sentido antihorario)");
          estadoActual = SETPOINT;     
        } else if(entradaUsuario == "X")
        {
          Serial.println("El motor ya se encuentra detenido.");
          estadoActual = DETENIDO;
        } else
        {
          Serial.println("Opción inválida.");
          estadoActual = DETENIDO;
        } 
      }
      break;
    case SETPOINT:
      if (Serial.available() > 0)
      {
        entradaUsuario = Serial.readStringUntil('\n');
        //Verificar la entrada del usuario
        if (entradaUsuario.charAt(0) == '+' || entradaUsuario.charAt(0) == '-')
        {
          String valorSetPoint = entradaUsuario.substring(1, entradaUsuario.length());
          if (isInteger(valorSetPoint))
          {
            if (entradaUsuario[0] == '+')
            {
              direccionActual = SENTIDO_HORARIO;
            } else
            {
              direccionActual = SENTIDO_ANTIHORARIO;
            }
            if (valorSetPoint.toInt() >=0 && valorSetPoint.toInt() <= 150)
            {
              setPoint = valorSetPoint.toInt();
              encenderMotor();
              estadoActual = EJECUCION;
            } else
            {
              Serial.println("Formato del set point incorrecto.");
              Serial.println("El valor ingresado se sale del rango.");
              estadoActual = DETENIDO;             
            }
          } else
          {
            Serial.println("Formato del set point incorrecto.");
            Serial.println("El valor ingresado después del signo no es numérico.");
            estadoActual = DETENIDO; 
          }
        } else
        {
          Serial.println("Formato del set point incorrecto.");
          Serial.println("El primer caracter debe ser el signo de dirección (+/-).");
          estadoActual = DETENIDO;
        }
      }
      break;
    case EJECUCION:
      if (Serial.available() > 0)
      {
        entradaUsuario = Serial.readStringUntil('\n');
        if (entradaUsuario == "S")
        {
          lcd.clear();
          Serial.println("Por favor ingrese el set point de la velocidad.");
          Serial.println("El valor ingresado debe ser un número de 0 a 150");
          Serial.println("Debe anteceder el valor el signo de velocidad:");
          Serial.println("+ (rotación en sentido horario)");
          Serial.println("- (rotación en sentido antihorario)");
          estadoActual = SETPOINT;
          apagarMotor();  
        } else if(entradaUsuario == "X")
        {
          lcd.clear();
          Serial.println("Deteniendo el motor.");
          apagarMotor();
          estadoActual = DETENIDO;
        } else
        {
          Serial.println("Opción inválida.");
        } 
      }

      if ((millis() - ultimaActualizacionEjecucion) >=  intervaloPrincipalEjecucion)
      {
        ultimaActualizacionEjecucion = millis();
        rpmCalculado =  10.0 * contadorPulsosA * (1.0 / pprE) * 60.0;

        contadorPulsosA = 0;
        contadorPulsosB = 0;

        error = setPoint - rpmCalculado;
        //Ecuación de diferencias
        cv = cv1 + (kp + kd/Tm)*error + (-kp + ki*Tm - 2*kd/Tm)*error1 + (kd/Tm)*error2;
        cv1 = cv;
        error2 = error1;
        error1 = error;

        //Saturar la salida del PID
        if (cv > 500.0)
        {
          cv = 500.0;
        }

        if (cv < 0.0)
        {
          cv = 0.0;
        }

        pwmValue = cv * (255.0 / 500.0);
        ledcWrite(velPin, pwmValue);   
      }
      if ((millis() - ultimaActualizacionMostrarMensaje) >=  intervaloPrincipalMensaje)
      {
        ultimaActualizacionMostrarMensaje = millis();

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SP (RPM): " + String(setPoint, 2));

        lcd.setCursor(0, 1);
        lcd.print("PV (RPM): " + String(rpmCalculado, 2));

        Serial.print("SP (RPM): ");
        Serial.println(setPoint);
        Serial.print("PV (RPM): ");
        Serial.println(rpmCalculado);
        Serial.print("ERROR (RPM): ");
        Serial.println(error);               
      }        
    break;
  } 
}
