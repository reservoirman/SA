#include <signal.h>
#include <stdio.h>
#include "messaging.h"

void message_daemon()
{
	while (1)
	{
		sleep(1);
		printf("Daemon running!\n");
	}
}

int main()
{
	signal(SIGINT, SIG_DFL);
	int result = fork();
	switch (result)
	{
		case -1:
			printf("%d: Starting daemon failed.  Please try again.\n", result);
			break;
		case 0:
			message_daemon();
			break;
		default:
			break;
	}

}