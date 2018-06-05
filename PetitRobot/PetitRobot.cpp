#include "PetitRobot.h"

PetitRobot::PetitRobot()
{ }

void PetitRobot::setup(Config_Robot config)
{
    ecran.setup(42);
    ecran.send("couleur"); // Commande de choix de couleur

    config.couleur = atoi(ecran.receive());

    Robot::setup(config);
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
    Serial << getAlpha() << endl;

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


void PetitRobot::commande_debug(String command, int param)
{
    if (command.equals("abeille"))
    {
        
    }
    else
      Robot::commande_debug(command, param);
}


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
