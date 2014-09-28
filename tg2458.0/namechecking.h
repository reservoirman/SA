#ifndef __NAMECHECKING_H__
#define __NAMECHECKING_H__

#define MAXNAMELENGTH 100

typedef enum { FILES, OBJECTS} NAME_TYPE;

//function prototype 
int namechecking_check(char *name, NAME_TYPE which);





#endif