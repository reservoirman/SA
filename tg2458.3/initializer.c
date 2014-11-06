#include <stdio.h>
#include <string.h>
#include "namechecking.h"

#define DEFAULT_FILE "userfile.txt"
#define DEFAULT_LENGTH strlen(DEFAULT_FILE)

int main(int argc, char **argv)
{
	int success = -1;
	FILE *file = fopen("initializer.txt", "w+");
	if (file >= 0)
	{
		if (argc == 2)
		{
			if (namechecking_check(argv[1], FILES) == 0)
			{
				int bytes_written = fwrite(argv[1], sizeof(char), strlen(argv[1]), file);
				if (bytes_written == strlen(argv[1]))
				{
					printf("User file will be %s\n", argv[1]);
					success = 0;	
				}
				
			}
		}
	}

	//writing the file didn't work, so write the default
	if (success == -1)
	{
		int bytes_written = fwrite(DEFAULT_FILE, sizeof(char), DEFAULT_LENGTH, file);
		if (bytes_written == DEFAULT_LENGTH)
		{
			printf("User file will be %s\n", DEFAULT_FILE);
			success = 0;	
		}
	}
	fclose(file);

	return success;

}