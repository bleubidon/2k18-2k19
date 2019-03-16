/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/

#include "SerialPort.h"
#include <stdlib.h>

SerialPort::SerialPort()
{
	connected = false;
}

SerialPort::~SerialPort()
{
	disconnect();
}

bool SerialPort::connect(char *portName, DWORD  baudRate)
{
	if (connected)
		return true;

	handler = CreateFileA(static_cast<LPCSTR>(portName),
				    GENERIC_READ | GENERIC_WRITE,
				    0,
				    NULL,
				    OPEN_EXISTING,
				    FILE_ATTRIBUTE_NORMAL,
				    NULL);
	if (handler != INVALID_HANDLE_VALUE)
	{
		DCB dcbSerialParameters = {0};

		if (GetCommState(handler, &dcbSerialParameters))
		{
			dcbSerialParameters.BaudRate = baudRate;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			connected = SetCommState(handler, &dcbSerialParameters);
			if (connected)
				PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
		}
	}

	return connected;
}

void SerialPort::disconnect()
{
	if (connected)
		CloseHandle(handler);
	connected = false;
}

int SerialPort::read(char *buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue > 0)
	{
		if (this->status.cbInQue > buf_size)
		{
			toRead = buf_size;
		}
		else
			toRead = this->status.cbInQue;
	}

	if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL))
		return bytesRead;

	return 0;
}

bool SerialPort::write(char *buffer, unsigned int buf_size)
{
	DWORD bytesSend;

	if (!WriteFile(this->handler, (void *)buffer, buf_size, &bytesSend, 0))
	{
		ClearCommError(this->handler, &this->errors, &this->status);
		return false;
	}
	else
		return true;
}

bool SerialPort::isConnected()
{
	return connected;
}
