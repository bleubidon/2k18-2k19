#include <Stepper.h>
#include <SPI.h>
#include <Radio.h>


//int equipe;
//initialisation equipe tests
int equipe = 1;
const int stepsPerRevolution = 200;

byte id = 102; //ID de l'arduino

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
			if (msg->source != 78)
				continue;
			Serial.print("Message reçu : \"");
			Serial.print(msg->text);
			Serial.println("\"");
      if (strcmp(msg->text,"jaune")==0){
        equipe = 0;
        radio.send(msg->source, "OK");
        return;
      }else if (strcmp(msg->text,"violette")==0){
        equipe =  1;
        radio.send(msg->source, "OK");
        return;
      }else{
        radio.send(msg->source, "no");
      }
		}
	}
}


void setup()
{
	Serial.begin(9600);

	const int motorSpeed = 100;
	myStepper.setSpeed(motorSpeed);

	radio.setup(id, 38, 42);
	
	
	
	waitTirette();
}

void loop()
{
  double beginTime = millis();
  Serial.print(beginTime);
  double currentTime = millis();
  /*while ((currentTime - beginTime) < 2000) {
    currentTime = millis();
  }*/
  while ((currentTime - beginTime) < 64000) {
    //Serial.println((currentTime - beginTime)/1000);
    currentTime = millis();
    if (equipe == 0) // Equipe jaune
      myStepper.step(stepsPerRevolution);
    else // Equipe violette
      myStepper.step(-stepsPerRevolution);
  }
  while(1) { 
    //myStepper.setSpeed(0);
   }
  
}
