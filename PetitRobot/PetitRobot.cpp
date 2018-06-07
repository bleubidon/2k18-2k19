#include "PetitRobot.h"

PetitRobot::PetitRobot()
{ }

void PetitRobot::setup(Config_Robot _config)
{
    ecran.setup();
    ecran.parse(42, "couleur 63488 1806"); // Commande de choix de couleur

    requestColor();

    Robot::setup(_config);
}

void PetitRobot::requestColor()
{
    String answer;
    do {
        delay(1000);
        answer = ecran.requestFrom(42, 1);
    }
    while (answer.charAt(0) == '\0');

    Serial << "Color is " << answer << endl;
}

void PetitRobot::setup_capteurs()
{
    codeuse.setup(11);
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
