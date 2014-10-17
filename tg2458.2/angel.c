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

static MessagingType _buffer;

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
	//run setsid to detach it from any particular session/terminal
	int set = setsid();
	if (set != -1)
	{
		umask(077);		//any files that the daemon creates, only the owner (root) can access them.

		//create message queue
		message_queue_id = messaging_init();
		signal(SIGINT, _closeMessageQueue);
		//if we created the message queue, start the daemon loop
		if (message_queue_id != -1)
		{
			printf("Message queue %d created! \n", message_queue_id);
			while (1)
			{
				int ret = msgrcv(message_queue_id, (void*)buffer, 0xFFFFFF, 0, 0);
				if (ret != -1)
				//MessagingType *output = messaging_receiveMessage();
				//if (output != NULL)
				{
					MessagingType *response = (MessagingType *)buffer;
					switch (response->message_type)
					{
						case OBJPUT:
							printf("ANGEL dump OBJPUT: %s\n", response->content);
							break;
						case OBJGET:
							printf("ANGEL dump OBJGET: %s\n", response->content);
							break;
						case OBJLIST:
							printf("ANGEL dump OBJLIST: %s\n", response->content);
							break;
						case OBJGETACL:
							printf("ANGEL dump OBJGETACL: %s\n", response->content);
							break;
						case OBJSETACL:
							printf("ANGEL dump OBJSETACL: %s\n", response->content);
							break;
						case OBJTESTACL:
							printf("ANGEL dump OBJTESTACL: %s\n", response->content);
							break;
						default:	
							printf("ANGEL dump string: %s\n", buffer);
							if (strncmp(buffer, "end", sizeof("end")) == 0)
							{
								_closeMessageQueue();
							}
							break;
					}
				}
				else
				{
					printf("Msgrcv failed! \n");//Return value %d\nErrno %d\n", strerror(errno), ret, errno);
					//break;
				}
				
			}		
		}
		else 
		{
			printf("Message queue creation failed! %s.  \nReturn value %d\nErrno %d\n", strerror(errno), message_queue_id, errno);
		}		
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