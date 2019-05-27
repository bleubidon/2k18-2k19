#include "Radio.h"

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

	DEBUG(printf_begin());

	radio.begin();
	radio.setAutoAck(1);
	radio.setRetries(1, 3);
	radio.enableDynamicPayloads();
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1, pipes[0]);
	radio.startListening();

	DEBUG(radio.printDetails());
}

Message *Radio::loop()
{
	if (radio.available())
	{
		radio.read(&msg, sizeof(Message));
		if (msg.destination == id)
			return &msg;
	}
	return nullptr;
}

void Radio::send(uint8_t dst, const char *msg)
{
	Message out;

	out.source = id;
	out.destination = dst;
	strncpy(out.text, msg, sizeof(out.text));

	radio.stopListening();
	delay(100);
	radio.write(&out, sizeof(Message));
	delay(100);
	radio.startListening();
}
