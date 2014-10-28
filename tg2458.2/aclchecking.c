#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "aclchecking.h"

static char _currentACL[5] = { 0 };
static char _star = '*';

static ACLERRNO _errno;
static char *r = "read the object",
			*w = "write to the object",
			*x = "execute the object",
			*p = "alter this object's ACL",
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
	int success = -1;
	char *acl_ref, *acl_line;
	
	//let's copy the acl to a local string so we can do work on it.
	char *acl_local = (char *)malloc(strlen(acl));
	strcpy(acl_local, acl);
	//break up the ACL by line:
	acl_ref = acl_local, acl_line = strtok_r(acl_local, "\n", &acl_ref);
	
	if (group_name == NULL)
	{
		group_name = &_star;
	}		

	while(acl_line != NULL)
	{
		//printf("The current ACL: %s\n", acl_line);
		//let's copy the line to a local string so we can work off that:
		char *acl_line_local = (char *)malloc(strlen(acl_line) + 1);
		strcpy(acl_line_local, acl_line);
		
		//break each line up into three tokens- user, group(s), and ops
		char *acl_line_ref = acl_line_local, *token = strtok_r(acl_line_local, ".", &acl_line_ref);
		
		if (token != NULL && token[0]== '*' || strncmp(token, user_name, strlen(user_name)) == 0)
		{
			token = strtok_r(NULL, "\t ", &acl_line_ref);
			if (token != NULL && token[0] == '*' || strncmp(token, group_name, strlen(group_name)) == 0)
			{
				token = strtok_r(NULL, "\n", &acl_line_ref);
				if (token != NULL && strstr(token, op) != NULL)
				{
					success = 0;
					strcpy(_currentACL, token);
					_errno = ACL_SUCCESS;
					break;
						
				}
				else
				{
					//this user of this group has permission to do the op!!!!
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
			if (_errno != ACL_USERCANNOT)
			{
				_errno = ACL_USERINVALID;
			}


		}
		free(acl_line_local);

		acl_line = strtok_r(NULL, "\n", &acl_ref);
	}	

	free(acl_local);
	return success;
}

char *aclchecking_getACL()
{
	return _currentACL;
}