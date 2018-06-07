#pragma once

#include <Robot.h>

#include <PositionDuPetitRobot.h>
#include <I2CParser.h>

#include <Encodeur.h>
#include <Gyroscope.h>
#include <Sick.h>


class PetitRobot: public Robot
{
	public:
    PetitRobot();

    void requestColor();
    
		void setup(Config_Robot _config);
		void setup_capteurs();
		void setup_actionneurs();

		void loop_capteurs();
		void loop_actionneurs();

    	void arret_actionneurs();
		

		float getX();
		float getY();
		float getAlpha();
		
	//private:
		PositionDuPetitRobot position;
    I2CParser ecran;
		
		/// Capteurs
			Encodeur codeuse;
			Gyroscope gyro;
         
			Sick sicks[2];

		/// Actionneurs
			//Rampe rampe;
			//Roues roues;
			//Abeille abeille;
};
