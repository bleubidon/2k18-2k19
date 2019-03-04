#include <cstdlib> //for random
#include <RF24/RF24.h>

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);

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

uint16_t timeout_ttl = 1000;

payload_request_t request;
payload_general_t answer;

void finish(bool success, int error, char * message)
{
    // printf("req: %d, ans: %d", request.number, answer.number);
    cout << "req" <<request.number;

	char status[6];
	if (success) {strcpy(status, "true");}
	else {strcpy(status, "false");}
	printf("{\"status\":\"%s\",\"code\":\"%d\",\"message\":\"%s\"}\n", status, error, message);
	
	if (success) {exit(0);}
	else {exit(1);}
}

int main(int argc, char** argv)
{
	if (argc!=3) {finish(false, 1, (char *) "Missing argument(s)");}

	radio.begin();
	radio.setAutoAck(1);
	radio.setRetries(15,15);
	radio.enableDynamicPayloads();
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);

	srand(time(NULL));
	request.destination = atoi( (char *) argv[1] );
	strncpy(request.message, (char *) argv[2], 14);
	request.number = (uint8_t) rand();

	radio.write(&request,sizeof(payload_request_t));
	radio.startListening();
	
	unsigned long started_waiting_at = millis();
	bool timeout = false;

	while ( !radio.available() && !timeout ) {
		if (millis() - started_waiting_at > timeout_ttl )
		timeout = true;}

	if ( timeout ) {finish(false, 2, (char *) "Timed-out");}
	radio.read( &answer, sizeof(payload_general_t));
	if (answer.number != request.number) {finish(false, 3, (char *) "Vérif échouée");}

	radio.stopListening();
	finish(true, 0, answer.message);
}




