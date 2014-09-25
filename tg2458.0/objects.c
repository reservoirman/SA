#include <stdio.h>
#include <unistd.h>	
#include <fcntl.h>	//for open
#include <string.h>
#include <stdlib.h>
#include "objects.h"

Object * array[100];		//change this to a linked list in the future

int objects_createUserList()
{
	int file = open("users.txt", O_RDONLY, 0);
	if (file >= 0)
	{
		array[0] = (Object *)malloc(sizeof(Object));
		array[0]->contents = (char *)malloc(128);	//hardcoded; will change
		array[0]->name = "users.txt";
		array[0]->user = "*";
		int nread = read(file, array[0]->contents, 128);

		array[0]->object_size = nread;
		printf("User list created!  Here is the list: \n%s\n", array[0]->contents);
		printf("\n list name: %s \n", array[0]->name);
		printf("\n list object size: %d\n", (int)array[0]->object_size);
	}

	return file;
}

int objects_listObjects(char *user)
{


}

int objects_listObjects(char *user, int option)
{

}
int objects_createObject(char *user, char *name, char *content)
{

}
int objects_getObject(char *user, char *name)
{
	
}