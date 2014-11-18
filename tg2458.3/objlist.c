#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>		//for uid_t and gid_t
#include <pwd.h>			//for struct passwd
#include <grp.h>			//for struct grp
#include "namechecking.h"
#include "objects.h"
#include "messaging.h"
#include <sys/msg.h>


int main (int argc, char **argv)
{
	int opt, show_length = 0, success = -1, result = -1;
	char *yep = "l", *nope = "0", *show = nope;

	while ((opt = getopt(argc, argv, ":l")) != -1)
	{
		switch (opt)
		{
			case 'l':
				show = yep;
				break;
			case ':':
			printf("Option needs a value\n");
			break;
			case '?':
			printf("Unknown option: %c\n", optopt);
			break;
			default:
			printf ("Invalid input.  Please try again.\n");
			break;

		}
	}

	//extract the user name
	uid_t uid = getuid();
	struct passwd *us = getpwuid(uid);

	//extract the group
	gid_t gid = getgid();
	struct group *gs = getgrgid(gid);

	//validate the user and group names
	if (us != NULL && gs != NULL)
	{
		//now to construct the request message to send to the angel
		result = messaging_sendRequest1(OBJLIST, us->pw_name, gs->gr_name, show);
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
			printf("OBJLIST error: message was not sent to daemon\n");
			
		}
	}
	else
	{
		printf("OBJLIST error: failed to obtain user name.  Try again.\n");
	}

	return success;
}