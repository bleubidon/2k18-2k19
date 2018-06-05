#pragma once

#include <Robot.h>

#include <PositionDuPetitRobot.h>

#include <Encodeur.h>
#include <Gyroscope.h>
#include <Sick.h>


class PetitRobot: public Robot
{
	public:
    PetitRobot();
    
		void setup_capteurs();
		void setup_actionneurs();

		void loop_capteurs();
		void loop_actionneurs();

    void arret_actionneurs();
		
		float getX();
		float getY();
		float getAlpha();

		void commande_debug(String command, int param);
		
	private:
		PositionDuPetitRobot position;
		
		/// Capteurs
			Encodeur codeuse;
			Gyroscope gyro;
         
			Sick sicks[2];

		/// Actionneurs
			//Rampe rampe;
			//Roues roues;
			//Abeille abeille;
};
