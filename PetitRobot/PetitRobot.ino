#include "PetitRobot.h"

Parser parser;
PetitRobot paschair;

void setup()
{
	Serial.begin(9600);

    parser.add("avancer", avancer);
    parser.add("tourner", tourner);

    
	Config_Robot config = {
		.couleur = GAUCHE,
		.dureeMatch = 90000L, // 90 secondes

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
	paschair.loop();
}

// Commands
void avancer(int argc, char **argv)
{
    if (argc < 2)
        return;

    int dist = atoi(argv[1]);
    
    Serial << "avance de " << dist << endl;
    paschair.setup_avancer(dist);
}

void tourner(int argc, char **argv)
{
    if (argc < 2)
        return;

    int angle = atoi(argv[1]);
    
    Serial << "rotation de " << angle << endl;
    paschair.setup_tourner(angle);
}

