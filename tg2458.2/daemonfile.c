#include <stdio.h>
#include <sys/msg.h>
int main()
{
	char input[100];
	int message_queue_id = 0;
	if (fork() == 0)
	{
		printf("Daemon activated!\n");
		message_queue_id = msgget(1004, IPC_CREAT);
		if (message_queue_id != -1)
		{
			printf("Message queue %d created! \n", message_queue_id);
		}
		while(1)
		{
			sleep(1);
			//if (fgets(input, 100, stdin) != NULL)
			if (fscanf(stdin, "%s", input) != 0)
			{
				printf("Echoing: %s\n", input);
			}
			else printf("Reading put failed!!!\n");
			if (strncmp(input, "^C", 2) ==0)
			{
				printf("DEAD!\n");
			}
			//printf("Daemon is alive!\n");
		}
	}

}



