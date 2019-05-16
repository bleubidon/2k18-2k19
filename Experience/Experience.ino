#include <Stepper.h>


const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
int stepCount = 0;  // number of steps the motor has taken;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);


void setup()
{
	Serial.begin(9600);
}

void loop()
{
	const int motorSpeed = 150;

	myStepper.setSpeed(motorSpeed);

	// step 1/100 of a revolution:
	myStepper.step(stepsPerRevolution / 100); // Direction roue jaune
	//myStepper.step(stepsPerRevolution / 100); // Direction roue grise
}
