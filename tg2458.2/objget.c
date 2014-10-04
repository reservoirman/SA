#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"
#include "namechecking.h"
#include "aclchecking.h"




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

	//if the user, group, and object are all fine and dandy:
	if (namechecking_validateInputs(user_name, group_name, argv[optind]) == 0)
	{	
		//check if this object already exists		
		char *acl = objects_readObject(user_name, argv[optind], ACL);
		if (acl != NULL)
		{
			//if so, check the acl to see if this user is 
			//allowed to read from the file
			if (aclchecking_isValid(user_name, group_name, acl, "r\0", (char *)"read from the object", 0) == 0)
			{
				//if we do have permission, let's go ahead and read the file
				char *contents = objects_readObject(user_name, argv[optind], DATA);
				if (contents != NULL)
				printf("%s", contents);
			}
		}
		else
		{
			printf("%s is not a valid object.  Please try again. \n", argv[optind]);
		}
	}

	free(user_name);
	free(group_name);



}
