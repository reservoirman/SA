#include <signal.h>		//for signal
#include <stdio.h>		//for printf
#include <string.h>
#include <sys/msg.h>	//for msgset, msgrcv, msgsnd, IPC_CREAT
#include <errno.h>		//for errno
#include <stdlib.h>		//for EXIT_SUCCESS
#include "messaging.h"

#define QUEUE_ID 1004
#define BUF_SIZE 100
static int message_queue_id = -1;
static char buffer[BUF_SIZE];

static void _closeMessageQueue()
{
	if(message_queue_id != -1)
	{
		if (msgctl(message_queue_id, IPC_RMID, NULL) == 0)
		{
			printf("closed the message queue!  Daemon exiting.\n");
		}
		else
		{
			printf("Message queue failed to close! Daemon exiting\n");
		}
	}
	exit(EXIT_SUCCESS);
}

//the one that runs as root
static void _messageDaemon()
{
	//create message queue
	message_queue_id = msgget(QUEUE_ID, IPC_CREAT);
	signal(SIGINT, _closeMessageQueue);
	//if we created the message queue, start the daemon loop
	if (message_queue_id != -1)
	{
		printf("Message queue %d created! \n", message_queue_id);
		while (1)
		{
			if (msgrcv(message_queue_id, (void*)buffer, BUF_SIZE, 0, 0) != -1)
			{
				printf("DAEMON: You wrote: %s\n", buffer);
				if (strncmp(buffer, "end", sizeof("end")) == 0)
				{
					_closeMessageQueue();
				}
			}
			else
			{
				printf("Msgrcv failed! %s.  \nReturn value %d\nErrno %d\n", strerror(errno), message_queue_id, errno);
			}
			
		}		
	}
	else 
	{
		printf("Message queue creation failed! %s.  \nReturn value %d\nErrno %d\n", strerror(errno), message_queue_id, errno);
	}

}

int main()
{
	

	int result = fork();
	switch (result)
	{
		case -1:
			printf("%d: Starting daemon failed.  Please try again.\n", result);
			break;
		case 0:
			_messageDaemon();
			break;
		default:
			break;
	}

}