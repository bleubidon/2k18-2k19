#pragma once

#include <RF24.h>

class Radio
{
	public:
		Radio(uint16_t pin_ce, uint16_t pin_csn);

		void setup(uint8_t _id, uint64_t _pipe_wr, uint64_t _pipe_rd);
		void loop();
		void send(uint8_t dst, const char *msg);

	private:
		RF24 radio;
		uint8_t id;
		uint64_t pipes[2];

		struct
		{
			uint8_t source;
			uint8_t destination;
			char message[14];
		} in;

		struct
		{
			uint8_t destination;
			char message[15];
		} out;
};
