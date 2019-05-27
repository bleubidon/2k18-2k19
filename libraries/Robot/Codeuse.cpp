/*!
 * \file Codeuse.cpp
 * \brief fonctions de communication avec les LS7366
 * \author Maximilien Courcelle
 * \author Adrien de Tocqueville
 * \version 2.0
 */

#include "Codeuse.h"

#include "SPI.h"


bool Codeuse::initializedSPI = false;

void Codeuse::setup(Codeuse::Config config)
{
	if (!initializedSPI)
	{
		initializedSPI = true;

		SPI.begin(); //start SPI library
		SPI.setBitOrder(MSBFIRST); //ordre des bits transferés, de gauche à droite ou de droite à gauche
		SPI.setDataMode(SPI_MODE0); //data on high or low clock, pas sûr!!!
		SPI.setClockDivider(SPI_CLOCK_DIV4); // data speed for SCK, pas sûr que ca soit 4Mhz le mieux
	}

	pin = config.pin;
	pinMode(pin, OUTPUT);

	ratio = (TWO_PI * config.radius) / (float)config.step_per_round;

	initCounter();
	delay(500);
}

float Codeuse::getDistance() const
{
	return getCounter() * ratio;
}

long Codeuse::getCounter() const
{
	byte inbyte = 0;
	long cntr = 0;

	uint8_t loadOtr = 0xE8;
	digitalWrite(pin, LOW);
	SPI.transfer(loadOtr);
	digitalWrite(pin, HIGH);

	uint8_t order = 0x68; // order = READ:01 + OTR:101 + Don't care:000
	uint8_t numberOfbyte = 4;

	digitalWrite(pin, LOW);
	SPI.transfer(order);
	for (int i = 0; i < numberOfbyte; i++)
	{
		inbyte = SPI.transfer(0x00);
		cntr = cntr << 8;
		cntr = cntr | inbyte;
	}
	digitalWrite(pin, HIGH);

	return cntr;
}

void Codeuse::clearCounter()
{
	uint8_t order = 0x20; // order = CLEAR CNTR
	digitalWrite(pin, LOW);
	SPI.transfer(order);
	digitalWrite(pin, HIGH);
}


void Codeuse::initCounter()
{
	// order = WRITE_MDR0
	// data = X1_QUADRATURE
	transfer(0x88, 0x03);


	// order = READ_MDR0
	transfer(0x48, 0x00);


	// order = WRITE_MDR1
	// data = 4_BYTES_COUNTER_MODE
	transfer(0x90, 0x00);


	// order = READ_MDR1
	transfer(0x50, 0x00);


	clearCounter();
}

void Codeuse::transfer(uint8_t order, uint8_t data)
{
	digitalWrite(pin, LOW);
	SPI.transfer(order);
	SPI.transfer(data);
	digitalWrite(pin, HIGH);
}
