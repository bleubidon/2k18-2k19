#include <I2CParser.h>

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';

I2CParser parser;

void setup()
{
    Serial.begin(9600);
    
    parser.setup();
    delay(1000);
    //parser.parse(42, "couleur 63488 1806");
    //parser.parse(42, "timer");
}

void loop()
{
  parser.loop(42);
}
