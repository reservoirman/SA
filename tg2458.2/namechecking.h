#ifndef __NAMECHECKING_H__
#define __NAMECHECKING_H__

#define MAXNAMELENGTH 100

typedef enum { USERS, FILES, OBJECTS, ACLS, OPS} NAME_TYPE;

//function prototype 
int namechecking_check(char *name, NAME_TYPE which);
int namechecking_validateACL(char *acl);
char *namechecking_copyName(char *name);
int namechecking_validateInputs(char *user, char *group, char *object);



#endif