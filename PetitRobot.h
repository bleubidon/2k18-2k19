#include "Robot.h"

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_Sensor.h> //utilisé pour le gyro
#include <Adafruit_BNO055.h> //utilisé pour le gyro
#include <utility/imumaths.h> //utilisé pour le gyro


class PetitRobot: public Robot
{
	public:
		PetitRobot();

		void setup_actionneurs();
		void loop_actionneurs();

		void debug_commande(char* command, int param);
		
	private:
		/// Capteurs
			// position
			Encodeur codeuse;
			Adafruit_BNO055 gyro;

			// detection
			Sick[2] sicks;

		/// Actionneurs
			Rampe rampe;
			Roues roues;
			Abeille abeille;
}