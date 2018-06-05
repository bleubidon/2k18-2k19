// author: Gregoire Fessard

// note: Si Wire.endTransmission() ne retourne jamais, il faut changer de carte.
// La bibliothèque Wire ne fonctionne pas sur certaines Arduino mega

#include <Wire.h>

const int SLAVE_ADDRESS = 42;


void setup_master()
{
    Serial.begin(9600);
    Wire.begin();
}

void loop_master()
{
    if (Serial.available() > 0)
    {
        if (send())
            Serial << "Successful transmission" << endl;
        else
            Serial << "Transmission error on master" << endl;

        // flush serial
        while (Serial.available() > 0)
            Serial.read();
    }
    
    request();
}

bool send()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(1);
    
    return Wire.endTransmission() == 0; // 0 means success
}

bool request()
{
    Wire.requestFrom(SLAVE_ADDRESS, 1);
    
    if (!Wire.available())
        return false;
        
    Serial << "Received " << Wire.available() << " bytes from slave" << endl;
        
    while (Wire.available())
    {
        char c = Wire.read();
        Serial << ">" << c << "<   ASCII: " << (int)c << endl;
    }
    
    Serial << endl;

    return true;
}
