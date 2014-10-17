#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>			//for memset
#include <stdio.h>
#include <errno.h>
#include "messaging.h"

#define MESSAGE_QUEUE_ID 	1004

#define SIZE(x)	strlen(x)

static int _msgid;
static char *_message;
static char *position;
static int message_length = 100;
static MessagingType _theMessage;

int messaging_init()
{
	_msgid = msgget(MESSAGE_QUEUE_ID, 0777 | IPC_CREAT);
	memset(&_theMessage, 0, sizeof(_theMessage));
	printf("MESSAGE SIZE = %lu!\n", sizeof(_theMessage));
	return _msgid;
}

static int _increment_position(int offset)
{
		position += offset;
		*position = '-';
		position++;
}

int messaging_sendMessage(MessageType type, char *u, char *g, char *o)
{
	int success = -1;

	_msgid = msgget(MESSAGE_QUEUE_ID, 0722 | IPC_CREAT);

	if (_msgid != -1)
	{
		/*
		memset(&_theMessage, 0, sizeof(_theMessage));
		message_length = 0;
		message_length += SIZE(u);
		message_length += SIZE(g);
		message_length += SIZE(o);
		message_length += SIZE(c) + 4;	//for the 4 hyphens used as delimiters
		_theMessage.message_type = 1004;
		_message = (char *)malloc (message_length);
		position = _message; 
		//position = _theMessage.message;
		strncpy(position, u, SIZE(u));
		_increment_position(SIZE(u));
		strncpy(position, g, SIZE(g));
		_increment_position(SIZE(g));
		strncpy(position, o, SIZE(o));
		_increment_position(SIZE(o));
		strncpy(position, c, SIZE(c));
		*/
		strncpy(_theMessage.user, u, SIZE(u));
		strncpy(_theMessage.group, g, SIZE(g));
		strncpy(_theMessage.object, o, SIZE(o));
		_theMessage.message_type = type;
		//_theMessage.message = _message;
		printf("MSGSND dump: %s\n", _theMessage.content);

		if (msgsnd(_msgid, (void *)&_theMessage, sizeof(_theMessage), 0) != -1)
		{
			success = 0;
		}	
		else
		{
			printf("Msgsnd failed! %s.\nErrno %d\n", strerror(errno), errno);
		}
	}
	else
	{
		printf("MESSAGING error: failed to get message queue.\n");
	}

	return success;
}

MessagingType * messaging_receiveMessage()
{
	MessagingType *output;

	memset(&_theMessage, 0, sizeof(_theMessage));

	_msgid = msgget(MESSAGE_QUEUE_ID, 0722 | IPC_CREAT);

	int ret = msgrcv(_msgid, (void*)&_theMessage, sizeof(_theMessage), 0, 0);
	if (ret == -1)
	{
		output = NULL;
	}
	else
	{
		output = &_theMessage;
	}
	printf("Msgrcv failed! %s.\nErrno %d\nMessage Length = %d\n", strerror(errno), errno, message_length / 2);
	return output;
}

void messaging_close()
{

				

}