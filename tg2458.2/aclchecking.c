#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "namechecking.h"
#include "aclchecking.h"

static char _currentACL[MAXNAMELENGTH + 20];

//checks if the desired action is permissible given the particular access control list
int aclchecking_isValidOp(char *user_name, char *group_name, char *acl, char *op, char *action, int surpress)
{
	int success = -1;
	//break it up into three tokens- user, group(s), and ops
	char *acl_ref =acl, *token = strtok_r(acl, ".", &acl_ref);
	if (token != NULL && strncmp(token, user_name, strlen(user_name)) == 0)
	{
		token = strtok_r(NULL, "\t ", &acl_ref);
		if (token != NULL && token[0] == '*' || strncmp(token, group_name, strlen(group_name)) == 0)
		{
			token = strtok_r(NULL, "\t\n ", &acl_ref);
			memset(_currentACL, 0, MAXNAMELENGTH + 20);
			strcpy(_currentACL, token);

			if (token != NULL && strstr(token, op) != NULL)
			{
				success = 0;
				
			}
			else
			{
				if (surpress == 0)
				{
					printf("User %s does not have permission to %s.\n", user_name, action);
				}

			}
			
		}
		else
		{
			if (surpress == 0)
			{
				printf("Group %s does not have permission to %s.\n", group_name, action);
			}

		}
	}
	else
	{
		if (surpress == 0)
		{
			printf("%s is not a valid user for this object.\n", user_name);
		}

	}

	return success;
}

char *aclchecking_getACL()
{
	return _currentACL;
}