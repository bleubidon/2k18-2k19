#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define RF_CE 2 //pin CE sur l'arduino
#define RF_CSN 4 //pin CSN sur l'arduino

RF24 radio(RF_CE,RF_CSN);

byte myID = 0x03; //ID de l'ardui

double pipes[] = {42, 38};

struct payload_request_t
{
	uint8_t number;
	uint8_t destination;
	char message[14];
};

struct payload_general_t
{
	uint8_t number;
	char message[15];
};

payload_request_t incoming;
payload_general_t outgoing;

void sendReply()
{
	Serial.print("Envoi de la réponse : \"");
	Serial.print(outgoing.message);
	Serial.println("\"");
	
	radio.stopListening();
	radio.openWritingPipe(pipes[1]);
	delay(10);
	radio.write(&outgoing,sizeof(payload_general_t)+1);
	delay(10);
	radio.startListening();
}

void setup() 
{
	Serial.begin(9600);
	printf_begin();
	radio.begin();
	radio.setAutoAck(1); 
	radio.setRetries(1,3);
	radio.enableDynamicPayloads();
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1, pipes[0]);
	radio.startListening();
	radio.printDetails();
}

void loop(void)
{
	if(radio.available()) {
		radio.read(&incoming, sizeof(payload_request_t));
		Serial.print("Message reçu : \"");
		Serial.print(incoming.message);
		Serial.println("\"");
			
		if (incoming.destination==myID) {
			outgoing.number = incoming.number;
			strcpy(outgoing.message, "OK");
			sendReply(); }}
	delay(100);
}
