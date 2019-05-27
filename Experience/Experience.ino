#include <Stepper.h>
#include <SPI.h>
#include <Radio.h>


int equipe;
const int stepsPerRevolution = 200;

byte id = 0x03; //ID de l'arduino

Stepper myStepper(stepsPerRevolution, 7, 9, 8, 10);
Radio radio(2, 4);

/*
 * code de retour:
 *	0 = Equipe jaune
 *	1 = Equipe violette
 */
void waitTirette()
{
	Message *msg;

	while (true)
	{
		if ((msg = radio.loop()))
		{
			Serial.print("Message reçu : \"");
			Serial.print(msg->text);
			Serial.println("\"");

			equipe = atoi(msg->text);
			radio.send(msg->source, "OK");
			return;
		}
	}
}


void setup()
{
	Serial.begin(9600);

	const int motorSpeed = 60;
	myStepper.setSpeed(motorSpeed);

	radio.setup(id, 38, 42);
	waitTirette();
}

void loop()
{
	if (equipe == 0) // Equipe jaune
		myStepper.step(stepsPerRevolution);
	else // Equipe violette
		myStepper.step(stepsPerRevolution);
}
