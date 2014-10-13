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

static char buffer[OBJECT_SIZE];

static void objput(char *objname)
{




}

static MessagingType _objput_message;

int main (int argc, char **argv)
{
	int opt, success = 0;
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
		printf("I AM %s AND I'M PART OF THE %s CREW\n", us->pw_name, gs->gr_name);
		if (argc == 2)
		{
			if (namechecking_validateInputs(us->pw_name, gs->gr_name, argv[1]) == 0)
			{
				//construct the message
				_objput_message.message_type = 1004;
				_objput_message.user = us->pw_name;
				_objput_message.group = gs->gr_name;
				_objput_message.object = argv[1];
				_objput_message.content = buffer;
				//send the message to the daemon
				int success = messaging_sendMessage(&_objput_message);

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


	//this should all go in the daemon:
	
	//if the user and group are valid, and if the names are all synctactically correct:
	if (namechecking_validateInputs(user_name, group_name, argv[optind]) == 0)
	{
		int success = -1;
		//check if this object already exists		
		char *acl = objects_readObject(user_name, argv[optind], ACL);
		if (acl != NULL)
		{

			//if so, check the acl to see if this user is 
			//allowed to write to the file
			if (aclchecking_isValid(user_name, group_name, acl, "w\0", (char *)"write to the object", 0) == 0)
			{
				success = 0;
			}
		}
		//if this object didn't exist before, 
		//creat the acl for it
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
	return 0;
}


