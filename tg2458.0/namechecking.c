#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include "namechecking.h"

#define OBJECT_REGEX    "[a-Z0-9_]+"
#define FILE_REGEX		"[a-Z0-9_]+.txt"

#define OBJECT_ERROR "%s is not a valid object name.  \nCan only contain letters, digits, and underscores.\n"
#define FILE_ERROR "%s is not a valid file name.  \nCan only contain letters, digits, and underscores, and must end in '.txt'\n"

static regex_t re;
regmatch_t match[2];

int namechecking_check(char *name, NAME_TYPE which)
{
	int success = -1;
	char *regex;
	const char *error_message;
	if (which == FILES)
	{
		regex = FILE_REGEX;
		error_message = FILE_ERROR;
	} 
	else 
	{
		regex = OBJECT_REGEX;
		error_message = OBJECT_ERROR;
	}
	if (regcomp(&re, regex, REG_EXTENDED | REG_ICASE) != 0)
	{
	    fprintf(stderr, "Failed to compile regex '%s'\n", regex);
	}

	if (regexec(&re, name, 2, match, REG_NOTBOL) == 0)
	{
		if (match[0].rm_eo - match[0].rm_so == strlen(name))
		{
			printf("<<%s>> string length: %d \n", name, (int)strlen(name));
			success = 0;
		}
		        
	}
    
	if (success == -1)
	{	
		printf(error_message, name);
	} 

	return success;

}