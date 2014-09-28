#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objects.h"

#define SIZE 100
char buffer[SIZE];

static void objput (char *username, char * objname)
{

}

int main (int argv, char **argc)
{

	int data_processed = read(0, buffer, SIZE);
	printf("OBJPUT: Here are the contents: %s\n", buffer);
	
	//if the user supplied all of the arguments
	if (argv == 4)
	{
		//print them out
		printf("OBJPUT: Created a new object %s for user %s.\n", argc[3], argc[2]);	
		objput(argc[2], argc[3]);
	}
	//otherwise, remind the user that he needs to do that
	else
	{
		printf("OBJPUT: Please enter user name and object name.\n");
	}

	objects_createUserList();
	//test out whether directly assigning pointer to pointer for user and name is ok,
	//across multiple objects.
	
	objects_createObject("ts", "holla", "what is going on?");
	objects_createObject("lk", "holla", "I dunno");
	objects_createObject("ts", "holla1", "time to party");
	objects_listObjects("ts", 0);

}