#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include "namechecking.h"
#include "objects.h"

#define USER_REGEX		"[a-Z0-9_]+"
#define OBJECT_REGEX    "[a-Z0-9_]+|([a-Z0-9_]+\\+[a-Z0-9_]+)"
#define FILE_REGEX		"[a-Z0-9_]+.txt"
#define ACL_REGEX		"[[:space:]]*(([a-Z0-9_]+|\\*).([a-Z0-9_]+|\\*)[[:blank:]]+[rwxpv]{1,5}[[:space:]]*)+"
#define OP_REGEX		"[rwxpv]";

#define USER_ERROR "ERROR: %s is not a valid user name.  \nCan only contain letters, digits, and underscores.\n"
#define OBJECT_ERROR "ERROR: %s is not a valid object name.  \nCan only contain letters, digits, and underscores. \nAn optional user name followed by a '+' may also prepend the object name.\n"
#define FILE_ERROR "ERROR: %s is not a valid file name.  \nCan only contain letters, digits, and underscores, and must end in '.txt'\n"
#define ACL_ERROR "ERROR: \"%s\" is not a valid ACL string.  \nCan only contain a valid object name or '*', \nfollowed by '.', followed by a valid object name or '*', \nfollowed by a space and any subset of r,w,x,p,v.\n"
#define OP_ERROR "ERROR: '%s' is not a valid operation.  Please enter 'r', 'w', 'x', 'p', or 'v'.\n"

static regex_t re;
regmatch_t match[2];

static char _star = '*';

int namechecking_check(char *name, NAME_TYPE which)
{
	int success = -1;
	char *regex;
	const char *error_message;
	switch (which)
	{
		case FILES:
			regex = FILE_REGEX;
			error_message = FILE_ERROR;
			break;
		case OBJECTS:
			regex = OBJECT_REGEX;
			error_message = OBJECT_ERROR;
			break;	
		case USERS:
			regex = USER_REGEX;
			error_message = USER_ERROR;
			break;
		case ACLS:
			regex = ACL_REGEX;
			error_message = ACL_ERROR;
			break;
		case OPS:
			regex = OP_REGEX;
			error_message = OP_ERROR;
			break;
	}
	if (regcomp(&re, regex, REG_EXTENDED | REG_ICASE | REG_NEWLINE) != 0)
	{
	    fprintf(stderr, "Failed to compile regex '%s'\n", regex);
	}

	if (regexec(&re, name, 2, match, REG_NOTBOL) == 0)
	{
		if (match[0].rm_eo - match[0].rm_so == strlen(name))
		{
			success = 0;
		}
		        
	}
    
	if (success == -1)
	{	
		printf(error_message, name);
	} 

	return success;

}

//this is used to copy the optarg which disappears
char * namechecking_copyName(char *name)
{
	char *user_name = (char *)malloc(strlen(name));
	strcpy(user_name, name);

	return user_name;
}

int namechecking_validateInputs(char *user_name, char *group_name, char *object_name)
{
	int success = -1;

	//if the user or object names are missing, bail out
	if (user_name == NULL)
	{
		printf("Please enter a user name, such as \"-u ts\"\n");
	}
	else if (object_name == NULL)
	{
		printf("Please enter an object name following the user name\n");
	}
	else 
	{
		//checking if the user, group, and object names are syntactically valid
		int isValid = 0;
		isValid |= namechecking_check(user_name, USERS);
		isValid |= namechecking_check(object_name, OBJECTS);
		if (group_name == NULL)
		{

			group_name = &_star;
		}
		else isValid |= namechecking_check(group_name, USERS);

		//if the user, group and object names are syntactically valid
		if (isValid == 0)
		{
			//check if the object name contains a separate user name
			char *plus = "+";
			char object_local[strlen(object_name)];
			strcpy(object_local, object_name);
			if (strstr(object_local, plus) != NULL)
			{
				char *user = strtok(object_local, plus);
				char *object = strtok(NULL, plus);
				
				//check if the user and group are valid
				printf("User = %s, Group Name = %s\n\n", user, group_name);
				ValidType result = objects_isValidUserGroup(user, group_name);
				
				switch (result)
				{
					case BADUSER:
						printf("%s is not a valid user!  Please try again.\n", user);
						break;
					case BADGROUP:
						printf("%s is not a valid group for %s!  Please try again.\n", group_name, user_name);
						break;
					case GOOD:
						success = 0;
					break;
				}
			}

			//check if the user and group are valid
			printf("User= %s, Group Name = %s\n", user_name, group_name);
			ValidType result = objects_isValidUserGroup(user_name, group_name);

			switch (result)
			{
				case BADUSER:
					printf("%s is not a valid user!  Please try again.\n", user_name);
					break;
				case BADGROUP:
					printf("%s is not a valid group for %s!  Please try again.\n", group_name, user_name);
					break;
				case GOOD:
					success = 0;
				break;
			}
		}

	}
	//free(user_name);
	//free(group_name);

	return success;
}