#include <Servo.h>
#include "helpers.h"

Servo bras_vigor;

int triggerPin = 9;
int vigorPin = 3;
int consigne_vigor = 172;

void setup() {
    Serial.begin(9600);
    pinMode(triggerPin, INPUT);
    bras_vigor.attach(vigorPin);
    bras_vigor.write(85);
    Serial << digitalRead(triggerPin) << endl;
}

void loop() {
	while(1) {
		if (digitalRead(triggerPin))
			break;
	}

	bras_vigor.write(consigne_vigor);
}
