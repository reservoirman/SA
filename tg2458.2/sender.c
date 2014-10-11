#include <sys/msg.h>	//for msgget, msgsnd
#include <stdio.h>		//for printf
#include <string.h>		//for strlen

#define MESSAGE_QUEUE_ID 	1004

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		int msgid = msgget(MESSAGE_QUEUE_ID, 0666 | IPC_CREAT);

		if (msgid != -1)
		{
			if (msgsnd(msgid, argv[1], strlen(argv[1]), 0) != -1)
			{
				printf("SENDER: message sent!\n");
			}	
		}
		
	}

}