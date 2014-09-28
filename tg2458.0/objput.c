#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"

#define SIZE 0xFFFFFF
char buffer[SIZE];


int main (int argv, char **argc)
{

	int data_processed = fread(buffer, sizeof(char), SIZE, stdin);

	printf("OBJPUT: %s\n", buffer);
	objects_createUserList();
	
	//if the user supplied all of the arguments
	if (argv == 6)
	{
		//print them out

		objects_createObject(argc[2], argc[5], buffer);
		printf("OBJPUT: Created a new object %s for user %s.\n", argc[5], argc[2]);	
	}
	else if (argv == 4)
	{
		//print them out
		objects_createObject(argc[2], argc[3], buffer);
		printf("OBJPUT: Created a new object %s for user %s.\n", argc[3], argc[2]);	
	}
	//otherwise, remind the user that he needs to do that
	else
	{
		printf("OBJPUT: Please enter user name and object name.\n");
	}

	
	//test out whether directly assigning pointer to pointer for user and name is ok,
	//across multiple objects.
	
	objects_createObject("ts", "holla", "what is going on?");
	objects_createObject("lk", "holla", "I dunno");
	objects_createObject("ts", "holla1", "time to party");
	objects_listObjects("ts", 0);

}