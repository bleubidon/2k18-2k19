#pragma once

#include <Robot.h>

#include <PositionDuPetitRobot.h>

#include <Encodeur.h>
#include <Gyroscope.h>
#include <Sick.h>

//#include <Ecran.h>


class PetitRobot: public Robot
{
	public:
    PetitRobot();
    
		void setup(Config_Robot _config);
		void setup_capteurs();
		void setup_actionneurs();

		void loop_capteurs();
		void loop_actionneurs();

    	void arret_actionneurs();
		

		float getX();
		float getY();
		float getAlpha();
		
	private:
		PositionDuPetitRobot position;
		//Ecran ecran;
		
		/// Capteurs
			Encodeur codeuse;
			Gyroscope gyro;
         
			Sick sicks[2];

		/// Actionneurs
			//Rampe rampe;
			//Roues roues;
			//Abeille abeille;
};
