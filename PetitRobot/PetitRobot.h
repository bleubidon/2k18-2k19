#include "Robot.h"

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <PositionDuPetitRobot.h>


class PetitRobot: public Robot
{
	public:
		PetitRobot(Config_Robot config);
		
		void setup_capteurs();
		void setup_actionneurs();

		void loop_capteurs();
		void loop_actionneurs();
		
		float getX();
		float getY();
		float getAlpha();

		void commande_debug(String command, int param);
		
	private:
		PositionDuPetitRobot position;

		/*
		/// Capteurs
			// position
			Encodeur codeuse;
			Gyroscope gyro;

			// detection
			Sick[2] sicks;

		/// Actionneurs
			Rampe rampe;
			Roues roues;
			Abeille abeille;
		*/
};