#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static void objget (char *username, char * objname)
{

}

int main (int argv, char **argc)
{
	//if the user supplied all of the arguments
	if (argv == 4)
	{
		//print them out
		printf("Retrieved object %s for user %s.\n", argc[3], argc[2]);	
		objget(argc[2], argc[3]);
	}
	//otherwise, remind the user that he needs to do that
	else
	{
		printf("Please enter user name and object name.\n");
	}

}