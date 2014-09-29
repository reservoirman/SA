#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"





int main (int argv, char **argc)
{
	//if the user supplied all of the arguments
	if (argv == 6)
	{
		//print them out

		char *contents = objects_readObject(argc[2], argc[5]);
		if (contents != NULL)
		{
			printf("OBJGET: Retrieved object %s for user %s.\n", argc[5], argc[2]);		
			if (strcmp("holla", "hollaback") == 0)
			{
				printf("holla = hollaback apparently\n");
			}
			printf("%s\n", contents);
		}
		

	}
	else if (argv == 4)
	{
		char *contents = objects_readObject(argc[2], argc[3]);
		if (contents != NULL)
		{
			printf("OBJGET: Retrieved object %s for user %s.\n", argc[3], argc[2]);		
			printf("%s\n", contents);
		}
	}
	//otherwise, remind the user that he needs to do that
	else
	{
		printf("OBJGET: Please enter user name and object name.\n");
	}	

}