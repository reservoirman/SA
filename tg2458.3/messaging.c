#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>			//for memset
#include <stdio.h>
#include <errno.h>
#include "messaging.h"

#define MESSAGE_QUEUE_ID 	10
#define MESSAGE_ACK_ID		 4

#define SIZE(x)	strlen(x)

static int _msgid, _msgid2;
static char *_message;
static char *position;
static int message_length = 100;
static MessagingType 		_theRequest;
static MessagingFinishType	_theFinish;



int messaging_init()
{
	int success = -1;

	if ((_msgid = msgget(MESSAGE_QUEUE_ID, 0777 | IPC_CREAT )) != -1 && 
		(_msgid2 = msgget(MESSAGE_ACK_ID, 0777 | IPC_CREAT )) != -1)
	{
		success = 0;
		memset(&_theRequest, 0, sizeof(_theRequest));
		memset(&_theFinish, 0, sizeof(_theFinish));
	}
	return success;
}

int messaging_isAlive()
{
	return (_msgid != -1 && _msgid2 != -1);
}

int messaging_sendRequest(MessageType type, char *u, char *g, char *o)
{
	int success = -1;

	if (messaging_init() != -1)
	{
		strncpy(_theRequest.request.user, u, SIZE(u));
		strncpy(_theRequest.request.group, g, SIZE(g));
		strncpy(_theRequest.request.object, o, SIZE(o));
		_theRequest.message_type = type;

		success = msgsnd(_msgid, (void *)&_theRequest, sizeof(MessagingType), 0);

		if (success == -1)
		{
			printf("MSGID = %u\n", _msgid);
			printf("messaging_sendRequest failed! %s.\nErrno %d\n", strerror(errno), errno);
		}
	}
	else
	{
		printf("MESSAGING error: failed to get message queue.\n");
	}

	return success;
}

int messaging_sendContent(char *c, int eof)
{
	int success = -1;

	if (messaging_init() != -1)
	{
		strncpy(_theRequest.data.content, c, SIZE(c));
		_theRequest.data.eof = eof;
		_theRequest.message_type = CONTENT;

		//printf("MSGSND dump: %s\n", _theRequest.data.content);

		if (msgsnd(_msgid, (void *)&_theRequest, sizeof(_theRequest), 0) != -1)
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

int messaging_sendFinished(int return_code)
{
	int success = -1;

	if (messaging_init() != -1)
	{
		_theFinish.return_code = return_code;
		_theFinish.message_type = FINISHED;

		if (msgsnd(_msgid2, (void *)&_theFinish, sizeof(_theFinish), IPC_NOWAIT) != -1)
		{
			success = 0;
		}	
		else
		{
			printf("messaging_sendFinished failed! %s.\nErrno %d\n", strerror(errno), errno);
		}
	}
	else
	{
		printf("MESSAGING error: failed to get message queue.\n");
	}

	return success;
}


MessagingType * messaging_receiveRequest()
{
	MessagingType *output = NULL;

	if (messaging_init() != -1)
	{
		int ret = msgrcv(_msgid, (void*)&_theRequest, sizeof(_theRequest), 0, 0);
		if (ret != -1)
		{
			output = &_theRequest;
		}
	}
	return output;
}

MessagingFinishType * messaging_receiveFinished()
{
	MessagingFinishType *finish = NULL;

	if (messaging_init() != -1)
	{
		int ret = msgrcv(_msgid2, (void*)&_theFinish, sizeof(_theFinish), 0, 0);
		if (ret == -1)
		{
			printf("messaging_receiveFinished failed! %s.\nErrno %d\nMessage Length = %d\n", strerror(errno), errno, message_length);
		}
		else
		{
			finish = &_theFinish;
		}
	}

	return finish;
}




int messaging_close()
{
	return (msgctl(_msgid, IPC_RMID, NULL) | msgctl(_msgid2, IPC_RMID, NULL));
}