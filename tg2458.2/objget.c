#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"
#include "namechecking.h"
#include "aclchecking.h"

int callObjGet(char *u1, char *u2, char *g, char *o)
{
		//check if this object already exists		
		char *acl = objects_readObject(u2, o, ACL);
		if (acl != NULL)
		{
			//if so, check the acl to see if this user is 
			//allowed to read from the file
			if (aclchecking_isValidOp(u1, g, acl, "r") == 0)
			{
				//if we do have permission, let's go ahead and read the file
				char *contents = objects_readObject(u2, o, DATA);
				if (contents != NULL)
				printf("%s", contents);
			}
			else
			{
				aclchecking_printErrno(u1, g, "r\0");
			}
		}
		else
		{
			printf("%s is not a valid object.  Please try again. \n", o);
		}
}


int main (int argc, char **argv)
{

	int opt, success = -1;
	char *user_name = NULL, *group_name = NULL;

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

	//if the user, group, and object are all fine and dandy:
	if (namechecking_validateInputs(user_name, group_name, argv[optind]) == 0)
	{	
		char *plus = "+", *second_user = NULL, *object_name = NULL;
		if (strstr(argv[optind], plus) != NULL)
		{
			second_user = strtok(argv[optind], plus);
			object_name = strtok(NULL, plus);
			callObjGet(user_name, second_user, group_name, object_name);
		}
		else
		{
			callObjGet(user_name, user_name, group_name, argv[optind]);
		}

	}

	free(user_name);
	free(group_name);


	return 0;
}
