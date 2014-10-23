#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"
#include "aclchecking.h"
#include "namechecking.h"

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
		//check if the ACL message is good
		if (namechecking_check(buffer, ACLS) == 0)
		{
			
		}

		//check if this object exists		
		char *acl = objects_readObject(user_name, argv[optind], ACL);
		if (acl != NULL)
		{
			//if so, check the acl to see if this user is 
			//allowed to write to the acl
			if (aclchecking_isValidOp(user_name, group_name, acl, "p\0", (char *)"change this object's ACL", 0) == 0)
			{
				objects_createObject(user_name, argv[optind], buffer, ACL);
			}
		}
		else
		{
			printf("%s is not a valid object.  Please try again. \n", argv[optind]);
		}
	}

}