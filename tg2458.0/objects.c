#include <stdio.h>
#include <unistd.h>	
#include <string.h>
#include <stdlib.h>
#include "objects.h"
#include "linkedlist.h"

static Item *object_list = 0;
static Object *current_object = 0;
static FILE *file;

int objects_createUserList()
{
	int success = -1;
	file = fopen("userfile.txt", "r");
	if (file != 0)
	{
		current_object = (Object *)malloc(sizeof(Object));
		current_object->contents = (char *)malloc(128);	//hardcoded; will change
		current_object->name = "userfile.txt";
		current_object->user = "*";
		int nread = fread(current_object->contents, sizeof(char), 128, file);

		current_object->object_size = nread;
		printf("User list created!  Here is the list: \n%s\n", current_object->contents);
		printf("\n list name: %s \n", current_object->name);
		printf("\n list object size: %d\n", (int)current_object->object_size);
		success = 0;
		object_list = linkedlist_newList((void *)current_object);
		fclose(file);
	}
	
	return success;
}

//instantiates a new object and then adds it to the linked list
int objects_createObject(char *iUser, char *iName, char *content)
{
	Object *new_object;
	int filenamelength = strlen(iUser) + strlen(iName) + 5;
	char *filename = (char *)malloc(filenamelength);
	//constructing the filename
	strcpy(filename, iUser);
	strcpy(filename + strlen(iUser), iName);
	strcpy(filename + strlen(iUser) + strlen(iName), ".txt\0");
	
	file = fopen(filename, "w+");
	if (file >= 0)
	{
		int bytes_written = fwrite(content, sizeof(char), strlen(content), file);
		if (bytes_written == strlen(content))
		{
			new_object = (Object *)malloc(sizeof(Object));
			new_object->contents = (char *)malloc(bytes_written);
			new_object->object_size = bytes_written;
			new_object->name = iName;
			new_object->user = iUser;
			new_object->contents = content;
			fclose(file);
			linkedlist_insertItem(object_list, (void *)new_object);
		}
	}

	

	return 0;
}

int objects_listObjects(char *iUser, int option)
{
	while (object_list != 0)
	{
		current_object = (Object *)object_list->item;
		if (strcmp(current_object->user, iUser) == 0)
		{
			printf("%s contents: %s\n", current_object->name, current_object->contents);
		}
		object_list = object_list->next;
	}
}

//getObject retrieves the object
int objects_getObject(char *user, char *name)
{

}