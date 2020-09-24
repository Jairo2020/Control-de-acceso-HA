/*
   Código creado por: Jairo Rohatan Zapata
   descripción:


   A. UNO    SENSOR AS608
   5V        V+
   2         TX
   3         RX
   GND       GND
*/

#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include "module.h"

SoftwareSerial mySerial(2, 3); // Crear Serial para Sensor  Rx ->3, TX ->2 del Arduino.
SoftwareSerial blue(6, 5);     // Crear serial para bluetooth Rx-> 5, Tx-> 6 del arduino.

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

LiquidCrystal_I2C lcd(0x27);

module modulos;

// Definición de puertos de arduino
// ENTRADAS
#define PUERTA_CERRADA 4 // micro suiche el cual dará la señal de cuando la puerta esté cerrada o abierta.
#define TIMBRAR 7        // Pulsador para timbrar.
#define SENSOROPEN 8     // Sensor ubicado en el lado de adentro de la casa para abrir la puerta.
#define SENSOR_PIR 9     // Funcionará en la noche, cunado haya una persona en frente se encendera la pantalla y una lampara.
#define ADDHUELLA 10     // Boton para añadir huellas localmente.
#define SENSOR_HUELLA 11 // Sensor que detectará cuando se va ingresar a la casa.

// Analogicas.
#define LDR A0
// SALIDAS
#define CERRADURA 12 // Cerradura electrónica.
#define TIMBRE 13    // Salida al timbre.

/**VARIABLES GLOBALES */

// Variables para convertir las entradas en datos de dos estados.
boolean sensorOpen;    // Leerá el estado del sensor del lado de adentro de la casa para abrir la puerta cuando ésta se encuentre cerrada.
boolean addHuella;     // Leerá el estado del pulsador de añadir huellas.
boolean puertaCerrada; // Leerá el estado de cuando la puerta está abierta o cerrada.
boolean sensorPir;     // Leerá el estado de la entrada del sensor pir.
boolean timbre;        // Leerá el estado de la entrada del piulsador para timbrar.
boolean sensorHuella;

int ldr;

// Control para la lógica
byte control[5] = {0, 0, 0, 0, 0};
/**
 * 
 * 
 * Índice 2 controla lo qu es la parte del bluetooth.
 */

unsigned long tiempo[5] = {5000, 3000, 4000, 0, 0}; //Manejar intervalos de tiempo.
/**
 * Índice 0 corresponde a 5s para hacer la lectura del sensor de huella.
 * Índice 1 corresponde a 3s para entrar en modo configuración.
 */
unsigned long tiempoPrevio = 0;
unsigned long millisActual = 0;

void setup()
{
    // Inicializa comunicación serial
    Serial.begin(9600);
    blue.begin(9600);
    finger.begin(57600);
    lcd.begin(20, 4);
    lcd.setBacklightPin(3, POSITIVE);
    lcd.setBacklight(HIGH);
    delayMicroseconds(5000);
    Serial.println("Conectando");
    while (!finger.verifyPassword())
    {
        Serial.println("...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("El sensor no se ha");
        lcd.setCursor(0, 1);
        lcd.print("inicaido con exito.");
        lcd.setCursor(0, 2);
        lcd.print("Reintentando");
        lcd.setCursor(0, 3);
        lcd.print("....");
        delay(1000);
    }

    Serial.println("Sensor inicaido correctamente");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor inicaido");
    lcd.setCursor(0, 1);
    lcd.print("correctamente");

    delay(2000);
    finger.LEDcontrol(0);

    // Definicaión de entradas y salidas
    pinMode(SENSOROPEN, INPUT);
    pinMode(SENSOR_PIR, INPUT);
    pinMode(PUERTA_CERRADA, INPUT);
    pinMode(SENSOR_HUELLA, INPUT);
    pinMode(ADDHUELLA, INPUT);
    pinMode(CERRADURA, OUTPUT);
    pinMode(TIMBRAR, INPUT);
    pinMode(TIMBRE, OUTPUT);

    lcd.clear();
}

void loop()
{
    sensorOpen = digitalRead(SENSOROPEN);
    puertaCerrada = digitalRead(PUERTA_CERRADA);
    addHuella = digitalRead(ADDHUELLA);
    sensorPir = digitalRead(SENSOR_PIR);
    timbre = digitalRead(TIMBRAR);
    sensorHuella = digitalRead(SENSOR_HUELLA);

    if (blue.available())
    {
        String data;
        data = blue.readString();
        Serial.println(data);
        if (data == "add")
        {
            control[2] = 1;
        }
        else if (data == "remove")
        {
            control[3] = 1;
        }
        else if (data == "start")
        {
            modulos.blueOF(&lcd, 1);
        }
    }

    if (sensorHuella == 1 && control[2] == 0 && control[1] == 0 && puertaCerrada == 0) // Se da acceso al usuario
    {
        control[0] = 1;
    }
    if (addHuella == 1 && sensorOpen == 1)
    {
        control[1] = 1;
    }
    if (control[0] == 1) // Verificando huelle para dar acceso, OJO poner estado de puerta para solo verfificar cuando esté cerrada.
    {
        lcd.setBacklight(HIGH);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("HUELLA DE ACCESO");

        unsigned long tiempoMillis = millis() + tiempo[0];
        while (tiempoMillis >= millis())
        {
            modulos.verifyUsers(&finger, &lcd, CERRADURA);
            puertaCerrada = digitalRead(PUERTA_CERRADA);
            if (puertaCerrada == 1)
            {
                digitalWrite(CERRADURA, LOW);
                modulos.candadoAC(&lcd, 0);
            }
            else if (puertaCerrada == 0)
            {
                modulos.candadoAC(&lcd, 1);
            }
        }
        finger.LEDcontrol(0);
        control[0] = 0;

        lcd.clear();
    }
    if (control[1] == 1) // Bloque para añadir huella localmente.
    {
        millisActual = millis();
        unsigned long tiempoMillis1 = millis() + tiempo[2];
        if (addHuella == 1 && millisActual - tiempoPrevio >= tiempo[1])
        {
            tiempoPrevio = millisActual;
            /* if (tiempoMillis >= millis())
            {
            } */
        }
        if (addHuella == 0 && tiempoMillis1 >= millis())
        {
            control[1] = 0;
        }
    }
    if (control[2] == 1) // Bloque para añadir huella por medio de app bluetooth.
    {
        byte id = blue.read();
        Serial.println(id);
        modulos.addFinger(&finger, &lcd, &blue, id);
    }

    if (puertaCerrada == 1)
    {
        modulos.candadoAC(&lcd, 0);
        digitalWrite(CERRADURA, LOW);
    }
    else if (puertaCerrada == 0)
    {
        modulos.candadoAC(&lcd, 1);
    }
    if (sensorOpen == 1 && addHuella == 0)
    {
        digitalWrite(CERRADURA, HIGH);
    }
    if(sensorPir == 1) // Poner condición de LDR
    {
        lcd.setBacklight(HIGH);
    }
    else if (sensorPir == 0)
    {
        lcd.setBacklight(LOW);
    }
    if (timbre == 1) // Se timbra
    {
        modulos.timbre(&lcd);
        digitalWrite(TIMBRE, HIGH);
    }
    else if (timbre == 0)
    {
        lcd.setCursor(18, 0);
        lcd.write(' ');
        digitalWrite(TIMBRE, LOW);
    }

    lcd.setCursor(4, 0);
    lcd.print("BIENVENIDOS");
    lcd.setCursor(2, 1);
    lcd.print("HOME-AUTOMATION");
    lcd.setCursor(0, 2);
    lcd.print("Familia Rohatan te");
    lcd.setCursor(0, 3);
    lcd.print("da la bienvenida");
}