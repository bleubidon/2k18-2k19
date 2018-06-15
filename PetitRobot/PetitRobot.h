#pragma once

#include <Robot.h>

#include <I2CParser.h>
#include <Sick.h>

struct Timer
{
    Timer(unsigned long _period) :
        last(0), period(_period)
    { }

    bool on()
    {
        unsigned long current = millis();

        if (current - last < period)
            return false;

        last = current;
        return true;
    }

    unsigned long last, period;
};

class PetitRobot : public Robot
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

	// private:
		Timer timer;
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
