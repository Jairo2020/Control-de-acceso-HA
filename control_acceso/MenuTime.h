#ifndef _MENUTIME_H_
#define _MENUTIME_H_
#include "module.h"
class MenuTime : public module
{
private:
    unsigned long retardoTiempo;
    byte contadorMenu = 0; // Variable para recorrer el menú.
    byte idHuella = 0;     // Variable que para éste caso servira como contador.
public:
    /* 
  * [índice] para el arreglo controlMenu.
  *  v
  * [0] Corresponde a control para el tiempo en que se está sosteniendo el boton de añadir huella.
  * Al sotarse ántes del tiempo requerido el tiempo configurado se reiniciará.
  * [1] Corresponde al control del tiempo que corresponde al acceso del submenú.
*/
    bool controlMenu[2] = {true, true};
    MenuTime(/* args */);
    ~MenuTime();

    /*!
    @method
    @param time tiempo dados en milisegundos.
    @param boton entrada del pulsador para controlar el menú.
    @param control [3] Para controlar saltos en los submenús.
     */
    bool entryTime(unsigned long time, bool boton, byte control[], LiquidCrystal_I2C *lcd, Adafruit_Fingerprint *finger, SoftwareSerial *blue);
    void contadorHuella(byte control[], LiquidCrystal_I2C *lcd);
    void iterMenuControl(byte control[], LiquidCrystal_I2C *lcd);
};

MenuTime::MenuTime(/* args */)
{
}

MenuTime::~MenuTime()
{
}


bool MenuTime::entryTime(unsigned long time, bool boton, byte control[], LiquidCrystal_I2C *lcd, Adafruit_Fingerprint *finger, SoftwareSerial *blue)
{
    if (control[0] == 0)
    {
        if (boton == 1)
        {
            if (controlMenu[0] == true)
            {
                retardoTiempo = time + millis();
                controlMenu[0] = false;
            }
            if (retardoTiempo <= millis() && controlMenu[0] == false)
            {
                // Serial.println("Muestra dentro de clase cuando se ha cumplido");
                control[0] = 1; // Controla el primer nivel
                // controlMenu[0] = true;
                lcd->clear();
                lcd->setCursor(6, 0);
                lcd->print("SETTINGS");
                lcd->setCursor(1, 1);
                lcd->print("Add");
                lcd->setCursor(1, 2);
                lcd->print("Delete");
                lcd->setCursor(1, 3);
                lcd->print("Exit");
                return true;
            }
        }
        else if (boton == 0 && retardoTiempo > millis() && controlMenu[0] == false)
        {
            controlMenu[0] = true;
            //Serial.println("Muestra dentro de clase cuando no se ha cumplido");
            return false;
        }
        else
        {
            return false;
        }
    }
    if (boton == 1 && control[1] == 0 && control[0] == 1)
    {

        // controlMenu[0] = true;
        if (controlMenu[0] == true)
        {
            retardoTiempo = millis() + time;
            // Serial.println("entra en el millis");
            controlMenu[0] = false;
        }
        if (retardoTiempo <= millis() && controlMenu[0] == false && control[3] == 2)
        {
            // Serial.println("Muestra dentro de clase cuando se ha cumplido");
            // Serial.println("Entra en el método");
            control[0] = 2;
            control[1] = 1; // Entra al tercer nivel del menú.
            control[3] = 0;
            controlMenu[0] = true;
            controlMenu[1] = true;
            switch (contadorMenu)
            {
            case 1:
                lcd->clear();
                lcd->setCursor(5, 0);
                lcd->print("ADD FINGER");
                lcd->setCursor(0, 1);
                lcd->print("- Selecciona indice");
                lcd->setCursor(0, 2);
                lcd->print("[   ] ");
                break;
            case 2:
                lcd->clear();
                lcd->setCursor(4, 0);
                lcd->print("REMOVE FINGER");
                lcd->setCursor(0, 1);
                lcd->print("- Selecciona indice");
                lcd->setCursor(0, 2);
                lcd->print("[   ] ");
                break;
            case 3:
                control[0] = 0;
                control[1] = 0;
                control[2] = 0;
                control[3] = 0;
                control[4] = 0;
                controlMenu[0] = true;
                controlMenu[1] = true;
                contadorMenu = 0;
                idHuella = 0;
                lcd->clear();
                lcd->setCursor(4, 0);
                lcd->print("BIENVENIDOS");
                lcd->setCursor(2, 1);
                lcd->print("HOME-AUTOMATION");
                lcd->setCursor(0, 2);
                lcd->print("Familia Rohatan te");
                lcd->setCursor(0, 3);
                lcd->print("da la bienvenida");
                break;
            }
            return true;
        }
        else if (boton == 1 && retardoTiempo > millis() && controlMenu[1] == true)
        {
            controlMenu[1] = false;
            // Serial.println("Muestra dentro de clase cuando no se ha cumplido");
            return false;
        }
    }
    else if (boton == 1 && control[1] == 1)
    {
        if (controlMenu[0] == true)
        {
            // Serial.println("Entra en el millis");
            retardoTiempo = millis() + time;
            controlMenu[0] = false;
        }
        else if (retardoTiempo <= millis() && controlMenu[0] == false && control[3] == 2)
        {
            // Serial.println(idHuella);
            if(contadorMenu == 1)
            {
                addFinger(finger,lcd,blue,&idHuella);
                idHuella = 0;
            }
            else if(contadorMenu == 2)
            {
                idHuella = 0;
            }
            control[0] = 1;
            control[1] = 0;
            control[2] = 1; // En ésta se guarda la poscisión de cuando etra al submenú.
            control[3] = 0;
            lcd->clear();
            lcd->setCursor(1, 1);
            lcd->print("Add");
            lcd->setCursor(1, 2);
            lcd->print("Delete");
            lcd->setCursor(1, 3);
            lcd->print("Exit");
            lcd->setCursor(6, 0);
            lcd->print("SETTINGS");
            return true;
        }
        else if (boton == 1 && retardoTiempo > millis() && controlMenu[1] == true)
        {
            // Serial.println("Entra cuando es 1");
            controlMenu[1] = false;
            return false;
        }
    }
}

void MenuTime::iterMenuControl(byte control[], LiquidCrystal_I2C *lcd)
{
    if (control[2] == 0)
    {
        contadorMenu++;
        // Serial.println("entra a aumentar" + String(contadorMenu));
        control[3] = 2;
        controlMenu[0] = true;
        controlMenu[1] = true;
        switch (contadorMenu)
        {
        case 1:
            lcd->setCursor(0, 1);
            lcd->print(">");
            break;
        case 2:
            lcd->setCursor(0, 1);
            lcd->print(" ");
            lcd->setCursor(0, 2);
            lcd->print(">");
            break;
        case 3:
            lcd->setCursor(0, 2);
            lcd->print(" ");
            lcd->setCursor(0, 3);
            lcd->print(">");
            break;
        default:
            contadorMenu = 1;
            lcd->setCursor(0, 1);
            lcd->print(">");
            lcd->setCursor(0, 3);
            lcd->print(" ");
            break;
        }
    }
    else
    {
        contadorMenu--; // Se disminuye en 1 ya que se mostraría el iterador en la siguiente fila del menú de opciones.
        control[2] = 0;
        //Serial.println("Entra para guardar registro");
    }
}

void MenuTime::contadorHuella(byte control[], LiquidCrystal_I2C *lcd)
{
    control[3] = 2;
    controlMenu[0] = true;
    controlMenu[1] = true;

    if (idHuella >= 127)
    {
        idHuella = 1;
        lcd->setCursor(1, 2);
        lcd->print("   ");
        lcd->setCursor(1, 2);
        lcd->print(idHuella);
    }
    else
    {
        idHuella++;
        lcd->setCursor(1, 2);
        lcd->print(idHuella);
    }
}

#endif // DEBUG
