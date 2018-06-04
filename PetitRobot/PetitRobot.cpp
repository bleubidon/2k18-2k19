#include "PetitRobot.h"

PetitRobot::PetitRobot(Config_Robot config) :
    Robot(config),
    position(0, 0, 0)
{
}

void PetitRobot::setup_capteurs()
{}

void PetitRobot::setup_actionneurs()
{}


void PetitRobot::loop_capteurs()
{}

void PetitRobot::loop_actionneurs()
{}


void PetitRobot::commande_debug(String command, int param)
{
    if (command.equals("abeille"))
    {
        
    }
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