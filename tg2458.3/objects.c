#include <stdio.h>		//for fopen, fclose, fgets
#include <unistd.h>		//for NULL
#include <string.h>		//for strcpy, memcpy, strtok_r, strcmp, strlen
#include <stdlib.h>		//for malloc, free
#include <dirent.h>		//for opendir, getcwd, readdir, closedir
#include <sys/stat.h>	//for stat, S_ISREG
#include <errno.h>
#include <sys/types.h>		//for uid_t and gid_t
#include <pwd.h>			//for struct passwd
#include <grp.h>			//for struct grp
#include "objects.h"
#include "linkedlist.h"
#include "namechecking.h"

static char buffer[OBJECT_SIZE];

static Item *object_list = NULL;
static Item **users_group_list = NULL;
static FILE *file = NULL;
static char line_from_users_list[MAXNAMELENGTH];

static int _createUserList()
{
	int success = -1, i = 0, j;
	char initializer_filename[MAXNAMELENGTH];

	//extract the user name
	uid_t uid = getuid();
	struct passwd *us = getpwuid(uid);

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
			if (file == NULL)
			{
				printf("!!!!Could not open %s!! %s\n", initializer_filename, strerror(errno));
			}
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
					i++; success = i;
				}
				//these calls crash the system for some reason
				//free(users_group_list);
				//fclose(file);

			}
			
		}
	}

	if (file == NULL)
	{
		printf("Could not open file to initialize user/group list!!! %s\n", strerror(errno));

	}
	return success;
}

ValidType objects_isValidUserGroup(char *iUser, char *iGroup)
{
	ValidType success = BADUSER;
	int i, total_users = _createUserList();
	if (total_users != -1)
	{
		for (i =0; i < total_users; i++)
		{
			//if iUser is found in the list of users
			if (strcmp((char *)users_group_list[i]->item, (char *)iUser) == 0)
			{
				//if iGroup is found in the list of groups for iUser
				if (iGroup[0] == '*' || linkedlist_searchItem(users_group_list[i], iGroup) == 0)
				{
					//iUser and iGroup are valid!
					success = GOOD;
				}
				else
				{
					success = BADGROUP;
				}

				break;
			}

		}	
	}

	return success;
}

static char * _constructFileName(char *user, char *name)
{
	int filenamelength = strlen(user) + strlen(name) + 4;
	char *filename = (char *)malloc(filenamelength);
	//constructing the filename
	strcpy(filename, user);
	strcpy(filename + strlen(user), name);
	strcpy(filename + strlen(user) + strlen(name), ".txt");

	return filename;
}

static char * _constructACLName(char *name)
{
	int aclnamelength = strlen(name) + 7;
	char *aclname = (char *)malloc(aclnamelength);
	
	strcpy(aclname, "acl");
	strcpy(aclname + 3, name);
	strcpy(aclname + 3 + strlen(name), ".txt");
	
	return aclname;
}

static char * _deconstructFileName(char *user, char *fileName)
{
	int object_name_length = strlen(fileName) - strlen(user) - 4;
	char *object_name = (char *)malloc(object_name_length);
	strncpy(object_name, fileName + strlen(user), object_name_length);
	object_name[object_name_length] = '\0';
	return object_name;
}

//instantiates a new object and then adds it to the linked list
int objects_createObject(char *iUser, char *iName, char *content, ObjectType which)
{
	int success = -1;

	char *filename = NULL;
	if (which == DATA)
	{
		filename = _constructFileName(iUser, iName);
	}
	else
	{
		filename = _constructACLName(iName);
	}
	//creating the file for the object
	// /chdir("enclave");
	file = fopen(filename, "w+");
	if (file > 0)
	{
		int bytes_written = fwrite(content, sizeof(char), strlen(content), file);
		if (bytes_written == strlen(content))
		{
			//create ACL here with default permissions u.* rwxpv
			success = 0;
		}
		fclose(file);
	}
	//creating the access control list for the object


	return success;
}


int objects_listObjects(char *iUser, int option)
{
	int success = -1;

	char directory_name[MAXNAMELENGTH];
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(getcwd(directory_name, MAXNAMELENGTH))) != NULL)
	{
		while ((entry = readdir(dp)) != NULL)
		{
			//obtain the stat structure for this directory entry
			stat(entry->d_name, &statbuf);
			if (S_ISREG(statbuf.st_mode))	//if this is a file
			{
				if (strncmp(iUser, entry->d_name, strlen(iUser)) == 0)
				{
					char *object_name = _deconstructFileName(iUser, entry->d_name);


					if (option == 1)
					{
						printf("%s \tsize = %d\n", object_name, (int)statbuf.st_size);				
					}
					else
					{
						printf("%s\n", object_name);
					}

					free(object_name);
				}
			}
		}
	
	}
	closedir(dp);
}

//readObject retrieves the object
char * objects_readObject(char *user, char *name, ObjectType which)
{
	char *success = NULL, *filename = NULL;
	//clear the buffer before a read:
	memset(buffer, 0, OBJECT_SIZE);
	if (which == DATA)
	{
		filename = _constructFileName(user, name);
	}
	else 
	{
		filename = _constructACLName(name);
	}
	file = fopen(filename, "r");
	if (file > 0)
	{
		int bytes_read = fread(buffer, sizeof(char), OBJECT_SIZE, file);
		success = buffer;
		fclose(file);
	}
	free(filename);
	return success;
}
