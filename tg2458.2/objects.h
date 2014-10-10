#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#define OBJECT_SIZE 0xFFFFFF

typedef enum {DATA, ACL} ObjectType;
typedef enum {BADUSER = -2, BADGROUP = -1, GOOD = 0} ValidType;  

ValidType objects_isValidUserGroup(char *user, char *group);
int objects_listObjects(char *user, int option);
int objects_createObject(char *user, char *name, char *content, ObjectType which);
char * objects_readObject(char *user, char *name, ObjectType which);


#endif
