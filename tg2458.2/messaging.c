#include <sys/msg.h>
#include <stdlib.h>
#include "messaging.h"

int messaging_sendMessage(char *message, size_t length)
{
	int success = -1;

	return success;
}

char * messaging_receiveMessage(size_t length)
{
	char *output = (char *)malloc(length);


	return output;
}