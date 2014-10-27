#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"
#include "aclchecking.h"
#include "namechecking.h"

int main (int argc, char **argv)
{
	int opt, success = 0;
	char *user_name = NULL, *group_name = NULL, *op_name = NULL;

	while ((opt = getopt(argc, argv, ":u:g:a:")) != -1)
	{
		switch (opt)
		{
			case 'u':
				user_name = namechecking_copyName(optarg);
				break;
			case 'g':
				group_name = namechecking_copyName(optarg);
				break;
			case 'a':
				op_name = namechecking_copyName(optarg);
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
		//if the op code is valid:
		if (namechecking_check(op_name, OPS) == 0)
		{
			//check if this object exists		
			char *acl = objects_readObject(user_name, argv[optind], ACL);

			//doublecheck that the content is good:
			if (acl != NULL && namechecking_check(acl, ACLS) == 0)
			{
				//call this to extract the ACL block
				if (aclchecking_isValidOp(user_name, group_name, acl, op_name) == -1)
				{
					printf("denied\n");
				}
				else
				{
					printf("allowed\n");
				}
			}
			else
			{
				printf("%s is not a valid object.  Please try again. \n", argv[optind]);
			}

		}

	}

}