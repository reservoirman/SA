#include <sys/msg.h>
#include <stdlib.h>
#include "messaging.h"

int messaging_sendMessage(MessagingType *message)
{
	int success = -1;

	return success;
}

MessagingType * messaging_receiveMessage()
{
	MessagingType *output = (MessagingType *)malloc(sizeof(MessagingType));


	return output;
}