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

static char buffer[OBJECT_SIZE];
static char chunk[CHUNK_LENGTH];

static void objput(char *objname)
{

}

static MessagingType _objput_message;

int callObjput(char *a, char *b, char *c, char *d)
{
	//this should all go in the daemon:
	
	//if the user and group are valid, and if the names are all synctactically correct:
	//if (namechecking_validateInputs(user_name, group_name, argv[optind]) == 0)
	/*
	{
		int success = -1;
		//check if an ACL for this object already exists		
		char *acl = objects_readObject(user_name, argv[optind], ACL);
		if (acl != NULL)
		{

			//if so, check the acl to see if this user is 
			//allowed to write to the file
			if (aclchecking_isValidOp(user_name, group_name, acl, "w\0") == 0)
			{
				success = 0;
			}
			//if the user is not allowed to write to the file,
			//check to see if the user is allowed to modify the acl to enable 
			//the user to do so:
			else if (aclchecking_isValidOp(user_name, group_name, acl, "p\0") == 0)
			{
				success = 0;
			}
		}
		//if the ACL for this object didn't exist before, 
		//create the ACL for it
		else
		{
			char *newacl = (char *)malloc(MAXNAMELENGTH + 20);
			sprintf(newacl, "%s.*\trwxpv\n", user_name);
			if(objects_createObject(user_name, argv[optind], newacl, ACL) == 0)
			{	
				free(newacl);
				success = 0;
				printf("OBJPUT: Created a new object ACL %s for user %s.\n", argv[optind], user_name);
			}	
		}

		//finally, if all is well, create/update the object itself:
		if (success == 0)
		{
			if (objects_createObject(user_name, argv[optind], buffer, DATA) == -1)
			{				
				printf("Failed to create object %s.  Please try again.\n", argv[optind]);
			}
			else
			{
				printf("OBJPUT: object written!\n");
			}
		}

	}

	free(user_name);
	free(group_name);*/
}

int main (int argc, char **argv)
{
	int opt, success = 1, result = -1;
	char *user_name = NULL, *group_name = NULL;
	
	fread(buffer, sizeof(char), OBJECT_SIZE, stdin);
	
	while ((opt = getopt(argc, argv, ":u:g:")) != -1)
	{
		switch (opt)
		{
			case 'u':
				user_name = namechecking_copyName(optarg);
				break;
			case 'g':
				group_name = namechecking_copyName(optarg);
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

	if (namechecking_validateInputs(user_name, group_name, argv[optind]) == 0)
	{
		int success = -1;
		//check if an ACL for this object already exists		
		char *acl = objects_readObject(user_name, argv[optind], ACL);
		if (acl != NULL)
		{

			//if so, check the acl to see if this user is 
			//allowed to write to the file
			if (aclchecking_isValidOp(user_name, group_name, acl, "w\0") == 0)
			{
				success = 0;
			}
			//if the user is not allowed to write to the file,
			//check to see if the user is allowed to modify the acl to enable 
			//the user to do so:
			else if (aclchecking_isValidOp(user_name, group_name, acl, "p\0") == 0)
			{
				success = 0;
			}
		}
		//if the ACL for this object didn't exist before, 
		//create the ACL for it
		else
		{
			char *newacl = (char *)malloc(MAXNAMELENGTH + 20);
			sprintf(newacl, "%s.*\trwxpv\n", user_name);
			if(objects_createObject(user_name, argv[optind], newacl, ACL) == 0)
			{	
				free(newacl);
				success = 0;
				printf("OBJPUT: Created a new object ACL %s for user %s.\n", argv[optind], user_name);
			}	
		}

		//finally, if all is well, create/update the object itself:
		if (success == 0)
		{
			if (objects_createObject(user_name, argv[optind], buffer, DATA) == -1)
			{				
				printf("Failed to create object %s.  Please try again.\n", argv[optind]);
			}
			else
			{
				printf("OBJPUT: object written!\n");
			}
		}

	}

	free(user_name);
	free(group_name);



	/*
	//extract the user name
	uid_t uid = getuid();
	struct passwd *us = getpwuid(uid);

	//extract the group
	gid_t gid = getgid();
	struct group *gs = getgrgid(gid);

	//validate the user and group names
	if (us != NULL && gs != NULL)
	{
		printf("I AM %s AND I'M PART OF THE %s CREW\n", us->pw_name, gs->gr_name);
		if (argc == 2)
		{
			if (namechecking_validateInputs(us->pw_name, gs->gr_name, argv[1]) == 0)
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
				printf("The last chunk: %s\n\n\n\n!!!\n\n\n\n", chunk);
				if (result == 0)
				{
					//now to construct the request message to send to the angel
					result = messaging_sendRequest(OBJPUT, us->pw_name, gs->gr_name, argv[1]);
					if (result == 0)
					{
						//if successful, send the content
						//divide the file into chunks of 0x1A74 bytes long and send them and rebuild the file
						MessagingFinishType *holla = messaging_receiveFinished();
						if (holla != NULL)
						{
							success = holla->return_code;	
							printf("Finito!!!! Return code = %d\n", success);
						}
						else
						{
							printf("What the, why was finish structure null???\n");
						}
						
						
					}
					else
					{
						printf("OBJPUT error: message was not sent to daemon\n");
						
					}
				}
				
				//the daemon should then create the file with root only permissions
			}
		}
		else
		{
			printf("OBJPUT error: please enter the object name.\n");
		}
	}
	else
	{
		printf("OBJPUT error: failed to obtain user/group names.  Try again.\n");
	}
*/
	return success;
}


