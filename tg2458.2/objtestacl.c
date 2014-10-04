#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"
#include "aclchecking.h"
#include "namechecking.h"

static char buffer[OBJECT_SIZE];
static char output[8];
static char *r = "r", *w = "w", *x = "x", *p = "p", *v = "v";

int _validateOpName(char *op_name)
{
	int success = -1;
	if (op_name == NULL)
	{
		printf("Please enter an operation to check on, e.g. '-a r' to check read permissions.\n");
	}
	else if ((strcmp(op_name, r) == 0) ||
		(strcmp(op_name, w) == 0) ||
		(strcmp(op_name, x) == 0) ||
		(strcmp(op_name, p) == 0) ||
		(strcmp(op_name, v) == 0))
	{
		success = 0;
	}
	else
	{
		printf("'%s' is not a valid operation.  Please enter 'r', 'w', 'x', 'p', or 'v'.\n", op_name);
	}

	return success;
}

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
		if (_validateOpName(op_name) == 0)
		{
			//check if this object exists		
			char *acl = objects_readObject(user_name, argv[optind], ACL);
			if (acl != NULL)
			{
				sprintf(output, "denied\n");
				//call this to extract the ACL block
				aclchecking_isValid(user_name, group_name, acl, "p\0", (char *)"change this object's ACL", 1);

				//get the ACL block
				acl = aclchecking_getACL();
				//check if it allows permissions
				if (strstr(acl, op_name))
				{
					sprintf(output, "allowed\n");
				}	
				printf("%s", output);
			}
			else
			{
				printf("%s is not a valid object.  Please try again. \n", argv[optind]);
			}

		}

	}

}