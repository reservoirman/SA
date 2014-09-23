#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include "namechecking.h"

#define tofind    "[a-Z0-9_]+"

static regex_t re;
regmatch_t match[2];

int namechecking_check(char *name)
{
	int success = -1;
	if (regcomp(&re, tofind, REG_EXTENDED | REG_ICASE) != 0)
	{
	    fprintf(stderr, "Failed to compile regex '%s'\n", tofind);
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
		printf("%s is not a valid name.  Can only contain letters, digits, and underscores.\n", name);
	} 

	return success;

}