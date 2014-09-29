#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "namechecking.h"
#include "objects.h"

static char buffer[OBJECT_SIZE];


int main (int argc, char **argv)
{
	int opt, show_length = 0, success = 0;
	char *user_name = NULL, *group_name = NULL;
	
	fread(buffer, sizeof(char), OBJECT_SIZE, stdin);
	while ((opt = getopt(argc, argv, ":u:g:")) != -1)
	{
		switch (opt)
		{
			case 'u':
				if (namechecking_check(optarg, OBJECTS) != 0)
				{
					return -1;
				}
				user_name = namechecking_copyName(optarg);
				break;
			case 'g':
				if (namechecking_check(optarg, OBJECTS) != 0)
				{
					return -1;
				}
				group_name = namechecking_copyName(optarg);
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
	else if (argv[optind] == NULL)
	{
		printf("Please enter an object name following the user name\n");
	}
	else if (namechecking_check(argv[optind], OBJECTS) == 0)
	{
		if (objects_createObject(user_name, argv[optind], buffer) == -1)
		{
			printf("%s is not a valid user.  Please try again.\n", user_name);
		}
		else printf("OBJPUT: Created a new object %s for user %s.\n", argv[optind], user_name);	
	}
	free(user_name);
	free(group_name);

}