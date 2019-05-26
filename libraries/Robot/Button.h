#pragma once

#include <Arduino.h>

enum class State {Up, Pressed, Down, Released};

class Button
{
	public:
		void setup(uint8_t _pin, unsigned long debounce_delay = 50)
		{
			pin = _pin;
			state = State::Up;
			last_state = 0;
			last_time = millis();
			trigger = debounce_delay;

			pinMode(pin, INPUT_PULLUP);
		}

		State loop()
		{
			if (state == State::Pressed)
				state = State::Down;
			else if (state == State::Released)
				state = State::Up;

			unsigned long now = millis();
			uint8_t val = digitalRead(pin);

			if (val != last_state)
				last_time = now;

			if (now - last_time > trigger)
			{
				if (val == HIGH && state == State::Down)
					state = State::Released;
				else if (val == LOW && state == State::Up)
					state = State::Pressed;
			}

			last_state = val;
			return state;
		}

		State state;

	private:
		uint8_t pin;
		uint8_t last_state;
		unsigned long last_time, trigger;
};
