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
static char chunk[CHUNK_LENGTH];

int main (int argc, char **argv)
{
	int opt, success = 1, result = -1;
	char *user_name = NULL, *group_name = NULL;

	fread(buffer, sizeof(char), OBJECT_SIZE, stdin);

	//extract the user name
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
				//divide the full up into 0x2F9-byte long chunks to send to the angel
				int i, reps = strlen(buffer) / CHUNK_LENGTH;
				char *current = buffer;
				for (i = 0; i < reps; i++)
				{
					strncpy(chunk, current, CHUNK_LENGTH);
					result = messaging_sendContent(chunk, 0);
					if (result == 0)
					{
						current += CHUNK_LENGTH;
					}
					else
					{
						break;
					}

				}
				//this is the last chunk to send, so set eof to 1
				strncpy(chunk, current, CHUNK_LENGTH);
				result = messaging_sendContent(chunk, 1);
				if (result == 0)
				{
					//now to construct the request message to send to the angel
					result = messaging_sendRequest(OBJSETACL, us->pw_name, gs->gr_name, argv[1]);
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
						printf("OBJSETACL error: message was not sent to daemon\n");
						
					}
				}
				
				//the daemon should then create the file with root only permissions
			}
		}
		else
		{
			printf("OBJSETACL error: please enter the object name.\n");
		}
	}
	else
	{
		printf("OBJSETACL error: failed to obtain user/group names.  Try again.\n");
	}
	return success;

}