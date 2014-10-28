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

static char buffer[OBJECT_SIZE];


int main (int argc, char **argv)
{
	int opt, success = -1, result = -1;

	uid_t uid = getuid();
	struct passwd *us = getpwuid(uid);

	//extract the group
	gid_t gid = getgid();
	struct group *gs = getgrgid(gid);

	//validate the user and group names
	if (us != NULL && gs != NULL)
	{
		if (argc == 2)
		{
			//if (namechecking_validateInputs(us->pw_name, gs->gr_name, argv[1]) == 0)
			{
					//now to construct the request message to send to the angel
					result = messaging_sendRequest(OBJGETACL, us->pw_name, gs->gr_name, argv[1]);
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
						printf("OBJGETACL error: message was not sent to daemon\n");
						
					}
				
				//the daemon should then create the file with root only permissions
			}
		}
		else
		{
			printf("OBJGETACL error: please enter the object name.\n");
		}
	}
	else
	{
		printf("OBJGETACL error: failed to obtain user/group names.  Try again.\n");
	}		

	return success;

}