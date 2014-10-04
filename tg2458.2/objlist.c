#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "namechecking.h"


int main (int argc, char **argv)
{
	int opt, show_length = 0;
	char *user_name = NULL;
	while ((opt = getopt(argc, argv, ":u:l")) != -1)
	{
		switch (opt)
		{
			case 'u':
				if (namechecking_check(optarg, OBJECTS) == 0)
				{
					user_name = namechecking_copyName(optarg);
				}
				break;
			case 'l':
				show_length = 1;
				break;
			case ':':
			printf("Option needs a value\n");
			break;
			case '?':
			printf("Unknown option: %c\n", optopt);
			break;
			default:
			printf ("Invalid input.  Please try again.\n");
			break;

		}
	}
	
	if (user_name == NULL)
	{
		printf("Please enter a user name, such as \"-u ts\"\n");
	}

	else if (objects_listObjects(user_name, show_length) == -1)
	{
		printf("%s is not a valid user.  Please try again.\n", user_name);
	}
	free(user_name);


}