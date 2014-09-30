#ifndef __NAMECHECKING_H__
#define __NAMECHECKING_H__

#define MAXNAMELENGTH 100

typedef enum { FILES, OBJECTS} NAME_TYPE;

//function prototype 
int namechecking_check(char *name, NAME_TYPE which);
char *namechecking_copyName(char *name);
int namechecking_validateInputs(char *user, char *group, char *object);



#endif