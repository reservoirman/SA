#include <stdio.h>
#include <string.h>
#include <unistd.h>			//for getuid
#include <stdlib.h>
#include <sys/types.h>		//for uid_t and gid_t
#include <pwd.h>			//for struct passwd
#include <grp.h>			//for struct grp
#include "namechecking.h"
#include "objects.h"
#include "messaging.h"
#include <sys/msg.h>

static char *buffer = NULL;
static char chunk[CHUNK_LENGTH];

int main (int argc, char **argv)
{

	int opt, success = -1, result = -1;
	
	//extract the user name
	uid_t uid = getuid();
	struct passwd *us = getpwuid(uid);

	//extract the group
	gid_t gid = getgid();
	struct group *gs = getgrgid(gid);

	//validate the user and group names
	if (us != NULL && gs != NULL)
	{
		char *passphrase = NULL; 

		while ((opt = getopt(argc, argv, ":k:")) != -1)
		{
			switch (opt)
			{
				case 'k':
					passphrase = optarg;
					break;
				case '?':
					printf("Unknown option: %c\n", optopt);
					break;
				default:
					printf ("Invalid input.  Please try again.\n");
					break;
			}
		}

		if (argc == 4)
		{
			
			//now to construct the request message to send to the angel
			result = messaging_sendRequest2(OBJGET, us->pw_name, gs->gr_name, argv[optind], passphrase);
			if (result == 0)
			{
				//if successful, send the content
				//divide the file into chunks of 0x1A74 bytes long and send them and rebuild the file
				MessagingFinishType *holla = messaging_receiveFinished();
				if (holla != NULL)
				{
					success = holla->return_code;	
				}
				else
				{
					printf("What the, why was finish structure null???\n");
				}
			}
			else
			{
				printf("OBJGET error: message was not sent to daemon\n");
				
			}

		}
		else
		{
			printf("OBJGET error: please enter the object name, followed by -k passphrase.\n");
		}
	}
	else
	{
		printf("OBJGET error: failed to obtain user/group names.  Try again.\n");
	}				


	return 0;
}
