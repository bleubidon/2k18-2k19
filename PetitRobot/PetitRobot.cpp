#include "PetitRobot.h"

PetitRobot::PetitRobot():
  timer(500)
{ }

void PetitRobot::setup(Config_Robot _config)
{
	ecran.setup();
	//requestColor();

	Robot::setup(_config);
}

void PetitRobot::requestColor()
{
  ecran.parse(42, "couleur 63488 1806"); // Commande de choix de couleur
	Serial << "Enter a color" << endl;
		
	char* answer;
	do {
		delay(1000);
		answer = ecran.requestFrom(42, 1);
	}
	while (*answer == '\0');

	Serial << "Color is " << answer << endl;
}

void PetitRobot::setup_capteurs()
{
	codeuse.setup(11, 800, 2.4);
	gyro.setup();

	sicks[DROITE].setup(37);
	sicks[GAUCHE].setup(38);
}

void PetitRobot::setup_actionneurs()
{


}

void PetitRobot::loop_capteurs()
{
	position.majPosition(codeuse.getCounter(), gyro.getOrientation());
		
	if (sicks[GAUCHE].active() || sicks[DROITE].active())
	{
		arret_moteurs();
	}
	else
		Moteur::stop = false;

	if (timer.on())
	{
	  static char command[15];
    
	  sprintf(command, "pos %d %d", (int)getAlpha(), (int)codeuse.getDistance());
	  
	  ecran.parse(42, command);
	}
}

void PetitRobot::loop_actionneurs()
{}


void PetitRobot::arret_actionneurs()
{}


float PetitRobot::getX()
{
	return position.getX();
}

float PetitRobot::getY()
{
	return position.getY();
}

float PetitRobot::getAlpha()
{
	return position.getAlpha();
}
