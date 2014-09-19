#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main (int argv, char **argc)
{
	//if the user supplied all of the arguments
	if (argv == 4)
	{
		//print them out
		printf("Listing objects including their metadata for user %s.\n", argc[2]);	
	}
	else if (argv == 3)
	{
		printf("Listing objects for user %s.\n", argc[2]);	
	}
	//otherwise, remind the user that he needs to do that
	else
	{
		printf("Please enter user name and object name.\n");
	}

}