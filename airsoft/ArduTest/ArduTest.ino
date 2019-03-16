#include <Parser.h>
#include "test.h"

Parser parser;

long start;
int consigne = 0;

void setup()
{
	Serial.begin(9600);

	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("stop", stop);
	parser.add("pid", set_pid);
}

void loop()
{
	parser.loop();

	if (consigne)
	{
		if (millis() > start + 1000)
		{
			Serial << "LOG:" << millis() << ',' << 5 << ',' << 10 << ',' << 20 << ',' << 25 << endl;
			start = millis();
			consigne--;
		}
		if (!consigne)
			Serial << "DEBUG:End of consigne" << endl;
	}
}

void set_pid(int argc, char **argv)
{
	if (argc != 5)
		return;
	if (argv[1][0] == '0')
		Serial.println("set pid dist");
	else
		Serial.println("set pid rot");
}

void dist(int argc, char **argv)
{
	if (argc == 2)
	{
		Serial.println("consigne dist");
		consigne = atoi(argv[1]);
		start = millis();
	}
}

void rot(int argc, char **argv)
{
	if (argc == 2)
		Serial.println("consigne rot");
}

void stop(int argc, char **argv)
{
	Serial << "stop" << endl;
}
