/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <windows.h>

class SerialPort
{
      public:
	SerialPort();
	~SerialPort();

	bool connect(char *portName, DWORD baudRate);
	void disconnect();

	int read(char *buffer, unsigned int buf_size);
	bool write(char *buffer, unsigned int buf_size);
	bool isConnected();

      private:
	HANDLE handler;
	bool connected;
	COMSTAT status;
	DWORD errors;
};

#endif // SERIALPORT_H
