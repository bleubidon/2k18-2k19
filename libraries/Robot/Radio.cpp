#include "Radio.h"

#define SERIAL_DEBUG

#include <nRF24L01.h>
#include <printf.h>

#include "helpers.h"


Radio::Radio(uint16_t pin_ce, uint16_t pin_csn):
	radio(pin_ce, pin_csn)
{}

void Radio::setup(uint8_t _id, uint64_t _pipe_wr, uint64_t _pipe_rd)
{
	id = _id;
	pipes[0] = _pipe_rd;
	pipes[1] = _pipe_wr;

	printf_begin();
	radio.begin();
	radio.setAutoAck(1);
	radio.setRetries(1, 3);
	radio.enableDynamicPayloads();
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1, pipes[0]);
	radio.startListening();

	DEBUG(radio.printDetails());
}

void Radio::loop()
{
	if (radio.available())
	{
		radio.read(&in, sizeof(in));

		Serial.print("Message reçu : \"");
		Serial.print(in.message);
		Serial.println("\"");

		if (in.destination == id)
			send(in.source, "OK");
	}
}

void Radio::send(uint8_t dst, const char *msg)
{
	out.destination = dst;
	strcpy(out.message, msg);

	Serial.print("Envoi de la réponse : \"");
	Serial.print(out.message);
	Serial.println("\"");

	radio.stopListening();
	delay(10);
	radio.write(&out, sizeof(out));
	delay(10);
	radio.startListening();
}
