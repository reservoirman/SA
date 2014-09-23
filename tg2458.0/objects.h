#ifndef __OBJECTS_H__
#define __OBJECTS_H__

typedef struct 
{
	char * user;
	size_t object_size;
	char * contents;
	char * name;

} Object;



int objects_createUserList();


#endif