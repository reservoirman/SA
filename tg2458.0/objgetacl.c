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
		//check if this object exists		
		char *acl = objects_readObject(user_name, argv[optind], ACL);
		if (acl != NULL)
		{
			//if so, check the acl to see if this user is 
			//allowed to read out the acl
			if (aclchecking_isValid(user_name, group_name, acl, "v\0", (char *)"view this object's ACL", 0) == 0)
			{
				//read out the ACL and print it to the console
				char *theACL = objects_readObject(user_name, argv[optind], ACL);
				if (theACL != NULL)
				{
					printf("%s\n", aclchecking_getACL());	
				}
				
			}
		}
		else
		{
			printf("%s is not a valid object.  Please try again. \n", argv[optind]);
		}
	}

}