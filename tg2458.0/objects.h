#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#define OBJECT_SIZE 0xFFFFFF

typedef struct 
{
	char * user;
	size_t object_size;
	char * contents;
	char * name;

} Object;


int objects_createUserList();
int objects_listObjects(char *user, int option);
int objects_createObject(char *user, char *name, char *content);
char * objects_readObject(char *user, char *name);


#endif