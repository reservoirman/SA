#include <stdio.h>
#include <unistd.h>	
#include <string.h>
#include <stdlib.h>
#include "objects.h"
#include "linkedlist.h"
#include "namechecking.h"

static char buffer[OBJECT_SIZE];

static Item *object_list = NULL;
static Object *current_object = NULL;
static Item **users_group_list = NULL;
static FILE *file = NULL;
static char line_from_users_list[MAXNAMELENGTH];

int objects_createUserList()
{
	int success = -1, i = 0, j;
	char initializer_filename[MAXNAMELENGTH];
	file = fopen("initializer.txt", "r");
	const char* space = " \n";
	users_group_list = (Item **)malloc(sizeof(Item **));


	if (file != NULL)
	{
		//read the filename from initializer
		if (fgets(initializer_filename, MAXNAMELENGTH, file) != NULL)
		{
			//open that file
			fclose(file);
			file = fopen(initializer_filename, "r");
			if (file != NULL)
			{
				while (fgets(line_from_users_list, MAXNAMELENGTH, file) != NULL)
				{
					//take each line, tokenize it and store the first token as user, and the 
					//subsequent tokens as the group(s) it belongs to 
					char *token;
					char *remaining = line_from_users_list;
					
					//obtain the user name
					token = strtok_r(line_from_users_list, space, &remaining);
					users_group_list[i] = linkedlist_newList((void *)token, strlen(token));
					//printf("OBJECTS: user = %s\n", token);
					while(token != NULL)
					{
						if ((token = strtok_r(NULL, space, &remaining)) != NULL)
						{
							linkedlist_insertItem(users_group_list[i], (void *)token, strlen(token));
							//printf("OBJECTS: group = %s\n", token);	
						}
						
					}
					i++;
				}
				fclose(file);
			}
			
		}

		//test code; should be removed later
		for (j = 0; j < i; j++)
		{
			printf("OBJECTS: user ");
			Item *aaa = users_group_list[j];
			while(aaa != NULL)
			{
				printf(" %s ", (char *)aaa->item);
				aaa = aaa->next;
			}
			printf("!!!\n");
		}

	}
	
	return success;
}

static char * _constructFileName(char *user, char *name)
{
	int filenamelength = strlen(user) + strlen(name) + 5;
	char *filename = (char *)malloc(filenamelength);
	//constructing the filename
	strcpy(filename, user);
	strcpy(filename + strlen(user), name);
	strcpy(filename + strlen(user) + strlen(name), ".txt\0");

	return filename;
}

//instantiates a new object and then adds it to the linked list
int objects_createObject(char *iUser, char *iName, char *content)
{
	Object *new_object;
	char *filename = _constructFileName(iUser, iName);
	
	file = fopen(filename, "w+");
	if (file >= 0)
	{
		int bytes_written = fwrite(content, sizeof(char), strlen(content), file);
		if (bytes_written == strlen(content))
		{
			fclose(file);
		}
	}

	return 0;
}

int objects_listObjects(char *iUser, int option)
{
	while (object_list != NULL)
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
char * objects_readObject(char *user, char *name)
{
	char *success = NULL;
	char *filename = _constructFileName(user, name);
	
	file = fopen(filename, "r");
	if (file >= 0)
	{
		int bytes_read = fread(buffer, sizeof(char), OBJECT_SIZE, file);
		success = buffer;
		fclose(file);
	}

	return success;
}