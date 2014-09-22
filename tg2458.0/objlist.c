#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>



int main (int argc, char **argv)
{
	int opt;


	while ((opt = getopt(argc, argv, ":if:lr")) != -1)
	{
		switch (opt)
		{
			case 'i':
			case 'l':
			case 'r':
				printf("Option: %c\n", opt);
				break;

			case 'f':
			printf("filename: %s\n", optarg);
			break;
			case ':':
			printf("Option needs a value\n");
			break;
			case '?':
			printf("Unknown option: %c\n", optopt);
			break;
			default:
			printf ("None of the above\n");
			break;

		}
	}

		for (; optind < argc; optind++)
		{
			printf("argument: %s\n", argv[optind]);
		}

}