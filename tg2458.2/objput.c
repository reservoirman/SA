#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "namechecking.h"
#include "objects.h"

static char buffer[OBJECT_SIZE];


int main (int argc, char **argv)
{
	int opt, success = 0;
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


