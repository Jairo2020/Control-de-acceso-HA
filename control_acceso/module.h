#ifndef _MODULE_H_
#define _MODULE_H_
class module
{
private:
    byte clearLcd[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00};

    byte alarm[8]{
        0x04,
        0x0E,
        0x0E,
        0x1F,
        0x04,
        0x00,
        0x00,
        0x00};

    byte candadoC[8] = {
        0x00,
        0x0E,
        0x11,
        0x11,
        0x1F,
        0x1F,
        0x1F,
        0x1F};

    byte candadoA[8] = {
        0x0E,
        0x11,
        0x11,
        0x01,
        0x1F,
        0x1F,
        0x1F,
        0x1F};

    byte caraFeliz1[8] = {
        0x00,
        0x00,
        0x01,
        0x03,
        0x06,
        0x0E,
        0x0E,
        0x0F};

    byte caraFeliz2[8] = {
        0x00,
        0x1F,
        0x1F,
        0x1F,
        0x0E,
        0x0E,
        0x0E,
        0x1F};

    byte caraFeliz3[8] = {
        0x00,
        0x00,
        0x10,
        0x18,
        0x0C,
        0x0E,
        0x0E,
        0x1E};

    byte caraFeliz4[8] = {
        0x0F,
        0x0F,
        0x0E,
        0x07,
        0x03,
        0x01,
        0x00,
        0x00};

    byte caraFeliz5[8] = {
        0x1F,
        0x1F,
        0x00,
        0x00,
        0x00,
        0x11,
        0x1F,
        0x00};

    byte caraFeliz6[8] = {
        0x1E,
        0x1E,
        0x0E,
        0x1C,
        0x18,
        0x10,
        0x00,
        0x00};

    // Se conforma la cara triste
    byte caraTriste1[8] = {
        0x00,
        0x00,
        0x01,
        0x03,
        0x06,
        0x0F,
        0x0E,
        0x0F};

    byte caraTriste2[8] = {
        0x00,
        0x1F,
        0x1F,
        0x1F,
        0x0E,
        0x1F,
        0x0E,
        0x1F};

    byte caraTriste3[8] = {
        0x00,
        0x00,
        0x10,
        0x18,
        0x0C,
        0x1E,
        0x0E,
        0x1E};

    byte caraTriste4[8] = {
        0x0F,
        0x0F,
        0x0F,
        0x07,
        0x02,
        0x01,
        0x00,
        0x00};

    byte caraTriste5[8] = {
        0x1F,
        0x1F,
        0x11,
        0x00,
        0x00,
        0x1F,
        0x1F,
        0x00};

    byte caraTriste6[8] = {
        0x1E,
        0x1E,
        0x1E,
        0x1C,
        0x08,
        0x10,
        0x00,
        0x00};

    byte blueOn[8] = {
        0x00,
        0x06,
        0x17,
        0x0E,
        0x04,
        0x0E,
        0x17,
        0x06};

    byte blueOff[8] = {
        0x00,
        0x06,
        0x15,
        0x0E,
        0x04,
        0x0E,
        0x15,
        0x06};

public:
    module();
    ~module();

    uint8_t addFinger(Adafruit_Fingerprint *finger, LiquidCrystal_I2C *lcd, SoftwareSerial *blue, byte *id);
    uint8_t verifyUsers(Adafruit_Fingerprint *finger, LiquidCrystal_I2C *lcd, uint8_t CERRADURA);
    /*!
    @funtion 
    @param  columna :coluna del lcd.
    @param fila: Fila de la lcd.
    @param cantDato: Cantidad de dato a borrar.
    */
    void clearLine(LiquidCrystal_I2C *lcd, byte columna, byte fila, byte cantDato);
    /*!
    @funtion 
    @param fila: Fila de la lcd.
    */
    void clearLine(LiquidCrystal_I2C *lcd, byte fila);
    void openDoor(uint8_t CERRADURA);
    void timbre(LiquidCrystal_I2C *lcd);
    void candadoAC(LiquidCrystal_I2C *lcd, boolean estado);
    void caraTF(LiquidCrystal_I2C *lcd, boolean estado);
    void printMessage(LiquidCrystal_I2C *lcd);
    /*!
        @funtion
        @param estado []: 1 bluetooth activo 0 bluetooth desactivado.
     */

    void blueOF(LiquidCrystal_I2C *lcd, boolean estado);
    byte clearSeg[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00};
};

module::~module()
{
}

module::module()
{
}

uint8_t module::addFinger(Adafruit_Fingerprint *finger, LiquidCrystal_I2C *lcd, SoftwareSerial *blue, byte *id)
{
    int p = -1;
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->println("Poner huella");
    while (p != FINGERPRINT_OK)
    {
        p = finger->getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            Serial.println("image take");
            lcd->clear();
            lcd->println("img obtenida");
            break;
        case FINGERPRINT_NOFINGER:
            Serial.println(".");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            break;
        default:
            Serial.println("Unknown error");
            break;
        }
    }
    // Ok succes
    p = finger->image2Tz(1);
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Image converted");
        lcd->clear();
        lcd->println("img convertida");

        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }
    Serial.println("Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger->getImage();
    }

    p = -1;
    Serial.println("Place same finger again");
    lcd->clear();
    lcd->println("Poner huella");
    lcd->print("nuevamente");
    while (p != FINGERPRINT_OK)
    {
        p = finger->getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            Serial.println("Image taken");
            break;
        case FINGERPRINT_NOFINGER:
            Serial.print(".");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            break;
        default:
            Serial.println("Unknown error");
            break;
        }
    }

    // OK success!

    p = finger->image2Tz(2);
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }

    // OK converted!
    Serial.print("Creating model for #");

    p = finger->createModel();
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Prints matched!");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication error");
        return p;
    }
    else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
        Serial.println("Fingerprints did not match");
        return p;
    }
    else
    {
        Serial.println("Unknown error");
        return p;
    }

    Serial.print("ID ");

    p = finger->storeModel(*id);
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Stored!");
        lcd->clear();
        lcd->print("Guardado");
        blue->print(1);
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication error");
        lcd->clear();
        lcd->print("Error comunicacion");
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        Serial.println("Could not store in that location");
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        Serial.println("Error writing to flash");
        return p;
    }
    else
    {
        Serial.println("Unknown error");
        return p;
    }

    return true;
}

void module::openDoor(uint8_t CERRADURA)
{
    digitalWrite(CERRADURA, HIGH);
}

uint8_t module::verifyUsers(Adafruit_Fingerprint *finger, LiquidCrystal_I2C *lcd, uint8_t CERRADURA)
{
    uint8_t p = finger->getImage();
    if (p != FINGERPRINT_OK)
        return -1;
    p = finger->image2Tz();
    if (p != FINGERPRINT_OK)
        return -1;
    p = finger->fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {
        if (p == FINGERPRINT_NOTFOUND)
        {
            Serial.println("Huella no coincidente");
            clearLine(lcd, 2, 0, 16);
            lcd->setCursor(0, 0);
            lcd->print("Acceso denegado");
            lcd->setCursor(1, 1);
            lcd->print("Huella no coincide");
            caraTF(lcd, 0);
            return -1;
        }
        else if (p == FINGERPRINT_PACKETRECIEVEERR)
        {
            Serial.println("Error de comunicación");
            lcd->setCursor(0, 1);
            lcd->print("Error de lectura");
            return -1;
        }
        return -1;
    }

    // found a match!
    Serial.print("Found ID #");
    Serial.print(finger->fingerID);
    Serial.print(" with confidence of ");
    Serial.println(finger->confidence);
    clearLine(lcd, 2, 0, 16);
    clearLine(lcd, 1);
    lcd->setCursor(0, 0);
    lcd->print("Huella aceptada");
    lcd->setCursor(0, 1);
    lcd->print("Bienvenid@ a casa");
    caraTF(lcd, 1);
    openDoor(CERRADURA);

    return finger->fingerID;
}

void module::clearLine(LiquidCrystal_I2C *lcd, byte columna, byte fila, byte cantDato)
{
    lcd->createChar(7, clearLcd);
    lcd->setCursor(columna, fila);
    lcd->write(byte(7));
    byte col = columna;
    for (size_t i = 0; i < cantDato; i++)
    {
        lcd->setCursor(col++, fila);
        lcd->write(byte(7));
    }
}

void module::clearLine(LiquidCrystal_I2C *lcd, byte fila)
{
    for (size_t i = 0; i <= 19; i++)
    {
        lcd->createChar(7, clearLcd);
        lcd->setCursor(i, fila);
        lcd->write(byte(7));
    }
}

void module::timbre(LiquidCrystal_I2C *lcd)
{
    lcd->createChar(7, alarm);
    lcd->setCursor(18, 0);
    lcd->write(byte(7));
}

void module::candadoAC(LiquidCrystal_I2C *lcd, boolean estado)
{
    if (estado == 1)
    {
        lcd->createChar(6, candadoA);
        lcd->setCursor(19, 0);
        lcd->write(byte(6));
    }
    else if (estado == 0)
    {
        lcd->createChar(6, candadoC);
        lcd->setCursor(19, 0);
        lcd->write(byte(6));
    }
}

void module::caraTF(LiquidCrystal_I2C *lcd, boolean estado)
{
    if (estado == 1) // Cara Feliz
    {
        lcd->createChar(0, caraFeliz1);
        lcd->createChar(1, caraFeliz2);
        lcd->createChar(2, caraFeliz3);
        lcd->createChar(3, caraFeliz4);
        lcd->createChar(4, caraFeliz5);
        lcd->createChar(5, caraFeliz6);

        lcd->setCursor(17, 2);
        lcd->write(byte(0));
        lcd->setCursor(18, 2);
        lcd->write(byte(1));
        lcd->setCursor(19, 2);
        lcd->write(byte(2));
        lcd->setCursor(17, 3);
        lcd->write(byte(3));
        lcd->setCursor(18, 3);
        lcd->write(byte(4));
        lcd->setCursor(19, 3);
        lcd->write(byte(5));
    }
    else if (estado == 0) // cara triste
    {
        lcd->createChar(0, caraTriste1);
        lcd->createChar(1, caraTriste2);
        lcd->createChar(2, caraTriste3);
        lcd->createChar(3, caraTriste4);
        lcd->createChar(4, caraTriste5);
        lcd->createChar(5, caraTriste6);

        lcd->setCursor(17, 2);
        lcd->write(byte(0));
        lcd->setCursor(18, 2);
        lcd->write(byte(1));
        lcd->setCursor(19, 2);
        lcd->write(byte(2));
        lcd->setCursor(17, 3);
        lcd->write(byte(3));
        lcd->setCursor(18, 3);
        lcd->write(byte(4));
        lcd->setCursor(19, 3);
        lcd->write(byte(5));
    }
}

void module::blueOF(LiquidCrystal_I2C *lcd, boolean estado)
{
    if (estado == 1)
    {
        lcd->createChar(6, blueOn);
        lcd->setCursor(17, 0);
        lcd->write(byte(6));
    }
    else if (estado == 0)
    {
        lcd->createChar(6, blueOff);
        lcd->setCursor(17, 0);
        lcd->write(byte(6));
    }
}

void module::printMessage(LiquidCrystal_I2C *lcd)
{
    lcd->clear();
    lcd->setCursor(4, 0);
    lcd->print("BIENVENIDOS");
    lcd->setCursor(2, 1);
    lcd->print("HOME-AUTOMATION");
    lcd->setCursor(0, 2);
    lcd->print("Familia Rohatan te");
    lcd->setCursor(0, 3);
    lcd->print("da la bienvenida");
}

#endif
