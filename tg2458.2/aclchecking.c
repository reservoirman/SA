#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "namechecking.h"
#include "aclchecking.h"

static char _currentACL[MAXNAMELENGTH + 20];

static ACLERRNO _errno;
static char *r = "read from the object",
			*w = "write to the object",
			*x = "execute the object",
			*p = "change this object's ACL",
			*v = "view this object's ACL";

void aclchecking_printErrno(char *user_name, char *group_name, char *op)
{	
	char *message;

	switch (op[0])
	{
		case 'r':
			message = r;
			break;
		case 'w':
			message = w;
			break;
		case 'x':
			message = x;
			break;
		case 'p':
			message = p;
			break;
		case 'v':
			message = v;
			break;
		default:
			message = "Invalid option, please try again.\n";
			break;
	}

	switch (_errno)
	{
		case ACL_USERCANNOT:
			printf("User %s does not have permission to %s.\n", user_name, message);
			break;
		case ACL_GROUPCANNOT:
			printf("Group %s does not have permission to %s.\n", group_name, message);
			break;
		case ACL_USERINVALID:
			printf("%s is not a valid user for this object.\n", user_name);
			break;
		case ACL_GROUPINVALID:
			printf("%s is not a valid group for this object.\n", group_name);
			break;
	}
}

//checks if the desired action is permissible given the particular access control list
int aclchecking_isValidOp(char *user_name, char *group_name, char *acl, char *op)
{
	int success = 0;
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

			if (token == NULL || strstr(token, op) == NULL)
			{
				success = -1;
				_errno = ACL_USERCANNOT;
				
			}
		}
		else
		{
			success = -1;
			_errno = ACL_GROUPCANNOT;
		}
	}
	else
	{
		success = -1;
		_errno = ACL_USERINVALID;

	}

	return success;
}

char *aclchecking_getACL()
{
	return _currentACL;
}