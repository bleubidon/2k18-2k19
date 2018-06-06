#include "PetitRobot.h"

Parser parser;
PetitRobot paschair;

void setup()
{
	Serial.begin(9600);

    parser.add("mv", deplacement);
    parser.add("test", testMoteurs);

    
	Config_Robot config = {
		.couleur = GAUCHE,
		.dureeMatch = -1, // 90000L // 90 secondes

		.pinTirette = 27,
		.pinMoteurs = {
			{4, 9, 6},
			{7, 8, 5}
		},

        .parser = &parser
	};


    paschair.setup(config);
}

void loop()
{
	//paschair.loop();
    paschair.consigneMoteurs(50, 50);
}

// Commands
void deplacement(int argc, char **argv)
{
    if (argc != 3)
        return;

    int val = atoi(argv[2]);

    if (!strcmp(argv[1], "t"))
    {
        Serial << "avance de " << val << endl;
        paschair.setup_avancer(val);
    }
    else if (!strcmp(argv[1], "r"))
    {
        Serial << "rotation de " << val << endl;
        paschair.setup_tourner(val);
    }
}

void testMoteurs(int argc, char **argv)
{
    int v = atoi(argv[1]);

    Serial << "consigne moteur: " << v << endl;
    paschair.consigneMoteurs(v, v);
}