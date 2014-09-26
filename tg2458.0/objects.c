#include <stdio.h>
#include <unistd.h>	
#include <fcntl.h>	//for open
#include <string.h>
#include <stdlib.h>
#include "objects.h"
#include "linkedlist.h"

static Item *object_list = 0;
static Object *current_object = 0;

int objects_createUserList()
{
	int file = open("users.txt", O_RDONLY);
	if (file >= 0)
	{
		current_object = (Object *)malloc(sizeof(Object));
		current_object->contents = (char *)malloc(128);	//hardcoded; will change
		current_object->name = "users.txt";
		current_object->user = "*";
		int nread = read(file, current_object->contents, 128);

		current_object->object_size = nread;
		printf("User list created!  Here is the list: \n%s\n", current_object->contents);
		printf("\n list name: %s \n", current_object->name);
		printf("\n list object size: %d\n", (int)current_object->object_size);
	}
	object_list = linkedlist_newList((void *)current_object);
	return file;
}

//instantiates a new object and then adds it to the linked list
int object_createObject(char *iUser, char *iName, char *content)
{
	Object *new_object;
	int filenamelength = strlen(iUser) + strlen(iName) + 5;
	char *filename = (char *)malloc(filenamelength);
	//constructing the filename
	strcpy(filename, iUser);
	strcpy(filename + strlen(iUser), iName);
	strcpy(filename + strlen(iUser) + strlen(iName), ".txt\0");
	
	int file = open("users.txt", O_RDWR | O_TRUNC);
	if (file >= 0)
	{
		int bytes_written = write(file, content, strlen(content));
		if (bytes_written == strlen(content))
		{
			new_object = (Object *)malloc(sizeof(Object));
			new_object->contents = (char *)malloc(bytes_written);
			new_object->object_size = bytes_written;
			new_object->name = iName;
			new_object->user = iUser;
			new_object->contents = content;
		}
	}

	linkedlist_insertItem(object_list, (void *)new_object);

	return 0;
}

int objects_listObjects(char *user, int option)
{

}

//getObject retrieves the object
int objects_getObject(char *user, char *name)
{

}