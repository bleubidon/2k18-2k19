/*!
 * \file Encodeur.cpp
 * \brief fonctions de communication avec les LS7366
 * \author Maximilien Courcelle
 * \author Adrien de Tocqueville
 * \version 2.0
 */

#include "Encodeur.h"

#include "SPI.h"


bool Encodeur::initializedSPI = false;

void Encodeur::setup(int _pin, int nb_pas_max, float rayon)
{
	if (!initializedSPI)
	{
		initializedSPI = true;

		SPI.begin(); //start SPI library
		SPI.setBitOrder(MSBFIRST); //ordre des bits transferés, de gauche à droite ou de droite à gauche
		SPI.setDataMode(SPI_MODE0); //data on high or low clock, pas sûr!!!
		SPI.setClockDivider(SPI_CLOCK_DIV4); // data speed for SCK, pas sûr que ca soit 4Mhz le mieux
	}
	
	pin = _pin;
	pinMode(pin, OUTPUT);

	ratio = 2.0*PI*rayon / nb_pas_max;

	initCounter();
	delay(500);
}

float Encodeur::getDistance() const
{
	return getCounter() * ratio;
}

long Encodeur::getCounter() const
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

void Encodeur::clearCounter()
{
	uint8_t order = 0x20; // order = CLEAR CNTR
	digitalWrite(pin, LOW);
	SPI.transfer(order);
	digitalWrite(pin, HIGH);
}


void Encodeur::initCounter()
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

void Encodeur::transfer(uint8_t order, uint8_t data)
{
	digitalWrite(pin, LOW);
	SPI.transfer(order);
	SPI.transfer(data);
	digitalWrite(pin, HIGH);
}