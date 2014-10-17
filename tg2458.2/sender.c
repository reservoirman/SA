#include <sys/msg.h>	//for msgget, msgsnd
#include <stdio.h>		//for printf
#include <string.h>		//for strlen
#include <errno.h>

#define MESSAGE_QUEUE_ID 	1004

typedef struct {
	long int message_type;
	char user[0xFF];
	char group[0xFF];
	char object[0xFF];
	char content[0xFFFFFF];
} MessagingType;

MessagingType holla;

static char buffer[0xFFFFFF];

int main(int argc, char **argv)
{
	
	printf("MESSAGE SIZE = %lu!\n", sizeof(holla));

	if (argc == 2)
	{
		int msgid = msgget(MESSAGE_QUEUE_ID, 0722 | IPC_CREAT);

		if (msgid != -1)
		{
			if (msgsnd(msgid, argv[1], strlen(argv[1]), 0) != -1)
			{
				printf("SENDER: message sent!\n");
			}	
		}
		
	}
	else
	{
		fread(buffer, sizeof(char), 0xFFFFFF, stdin);
		printf("BUFFER!!!\n%s\n", buffer);
		int msgid = msgget(MESSAGE_QUEUE_ID, 0722 | IPC_CREAT);

		if (msgid != -1)
		{
			if (msgsnd(msgid, buffer, strlen(buffer) + 40, 0) != -1)
			{
				printf("SENDER: message sent!\n");
			}	
			else
			{
				printf("Msgsnd failed! %s.\nErrno %d\nMessage Length = %lu\n", strerror(errno), errno, strlen(buffer));
			}
		}
	}

}