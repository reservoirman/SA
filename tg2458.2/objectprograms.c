#include <stdio.h>
#include <string.h>
#include <unistd.h>			//for getuid
#include <stdlib.h>
#include <sys/types.h>		//for uid_t and gid_t
#include <pwd.h>			//for struct passwd
#include <grp.h>			//for struct grp
#include "namechecking.h"
#include "objects.h"
#include "objectprograms.h"
#include "messaging.h"
#include <sys/msg.h>

static char buffer[OBJECT_SIZE];

static MessagingType _objput_message;

int objects_objput(char *a, char *b, char *c, char *d)
{
	//this should all go in the daemon:
	
	//if the user and group are valid, and if the names are all synctactically correct:
	//if (namechecking_validateInputs(user_name, group_name, argv[optind]) == 0)
	/*
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
	free(group_name);*/
}

int objects_objget(char *a, char *b, char *c, char *d)
{

}

int objects_objlist(char *a, char *b, char *c, char *d)
{
	
}

int objects_objsetacl(char *a, char *b, char *c, char *d)
{
	
}

int objects_objgetacl(char *a, char *b, char *c, char *d)
{
	
}

int objects_objtestacl(char *a, char *b, char *c, char *d)
{
	
}

