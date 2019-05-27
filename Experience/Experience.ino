#include <Stepper.h>
#include <SPI.h>
#include <Radio.h>


int equipe;
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
int stepCount = 0;  // number of steps the motor has taken;

Stepper myStepper(stepsPerRevolution, 7, 9, 8, 10);
Radio radio(2, 4);

/*
 * code de retour:
 *	0 = Equipe violette
 *	1 = Equipe jaune
 */
/*
int waitTirette()
{
	Message *msg;

	while (true)
	{
		if ((msg = radio.loop()))
		{
			Serial.print("Message reçu : \"");
			Serial.print(msg->text);
			Serial.println("\"");

			radio.send(msg->source, "OK");
			//return 1;
		}
	}
	return -1;
}

void setup()
{
	Serial.begin(9600);

	const int motorSpeed = 60;
	myStepper.setSpeed(motorSpeed);

	radio.setup(0x03, 38, 42);
	equipe = waitTirette();
}

void loop()
{

	// step 1/100 of a revolution:
	myStepper.step(stepsPerRevolution); // Direction roue jaune
	//myStepper.step(stepsPerRevolution / 100); // Direction roue grise
}
*/
byte id = 0x03; //ID de l'ardui

double pipes[] = {42, 38};


struct payload_general_t
{
	uint8_t number;
	char message[15];
};

Message incoming;
payload_general_t outgoing;

void sendReply()
{
	Serial.print("Envoi de la réponse : \"");
	Serial.print(outgoing.message);
	Serial.println("\"");
	
	radio.radio.stopListening();
	radio.radio.openWritingPipe(pipes[1]);
	delay(10);
	radio.radio.write(&outgoing,sizeof(payload_general_t)+1);
	delay(10);
	radio.radio.startListening();
}
void sendReply1(uint8_t dest, const char *msg)
{
	outgoing.number = dest;
	strcpy(outgoing.message, msg);

	Serial.print("Envoi de la réponse : \"");
	Serial.print(outgoing.message);
	Serial.println("\"");
	
	radio.radio.stopListening();
	radio.radio.openWritingPipe(pipes[1]);
	delay(10);
	radio.radio.write(&outgoing,sizeof(payload_general_t)+1);
	delay(10);
	radio.radio.startListening();
}
void sendReply2(uint8_t dest, const char *msg)
{
	outgoing.number = dest;
	strcpy(outgoing.message, msg);

	Serial.print("Envoi de la réponse : \"");
	Serial.print(outgoing.message);
	Serial.println("\"");
	
	radio.radio.stopListening();
	delay(10);
	radio.radio.write(&outgoing,sizeof(payload_general_t));
	delay(10);
	radio.radio.startListening();
}
void sendReply3(uint8_t dest, const char *msg)
{
	outgoing.number = dest;
	strcpy(outgoing.message, msg);

	Message *msg = (Message*)outgoing;

	Serial.print("Envoi de la réponse : \"");
	Serial.print(outgoing.message);
	Serial.println("\"");
	
	radio.radio.stopListening();
	delay(10);
	radio.radio.write(msg, sizeof(Message));
	delay(10);
	radio.radio.startListening();
}
void sendReply4(uint8_t dest, const char *msg)
{
	outgoing.number = dest;
	strcpy(outgoing.message, msg);

	Message msg;
	msg.source = dest;
	msg.destination = 'o';
	strcpy(msg.text, msg);

	Serial.print("Envoi de la réponse : \"");
	Serial.print(outgoing.message);
	Serial.println("\"");
	
	radio.radio.stopListening();
	delay(10);
	radio.radio.write(&msg, sizeof(Message));
	delay(10);
	radio.radio.startListening();
}

void setup() 
{
	Serial.begin(9600);

	Serial.println(sizeof(Message));
	Serial.println(sizeof(payload_general_t));

	radio.setup(id, pipes[1], pipes[0]);
}

void loop(void)
{
	if(radio.radio.available()) {
		radio.radio.read(&incoming, sizeof(Message));
		Serial.print("Message reçu : \"");
		Serial.print(incoming.text);
		Serial.println("\"");
			
		if (incoming.destination==id)
		{
			//Goal
			//radio.send(incoming.source, "OK");

			//Default
			outgoing.number = incoming.source;
			strcpy(outgoing.message, "OK");
			sendReply();

			//Tries
			//sendReply1(incoming.source, "OK");
		}
	}
	delay(100);
}
