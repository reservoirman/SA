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
	int opt, success = -1, result = -1;
	char *user_name = NULL, *group_name = NULL, *op_name = NULL;

	while ((opt = getopt(argc, argv, ":a:")) != -1)
	{
		switch (opt)
		{
			case 'a':
				op_name = namechecking_copyName(optarg);
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
	//if the user and group are valid, and if the names are all synctactically correct:
	//if (namechecking_validateInputs(us->pw_name, gs->gr_name, argv[optind]) == 0)
	{
		//if the op code is valid:
		if (op_name != NULL && namechecking_check(op_name, OPS) == 0)
		{

			result = messaging_sendContent(op_name, 1);
			if (result == 0)
			{
				//now to construct the request message to send to the angel
				result = messaging_sendRequest(OBJTESTACL, us->pw_name, gs->gr_name, argv[optind]);
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
					printf("OBJTESTACL error: message was not sent to daemon\n");
					
				}
			}
		}
	}
	}
	else
	{
		printf("OBJTESTACL error: failed to obtain user/group names.  Try again.\n");
	}	


	return success;

}