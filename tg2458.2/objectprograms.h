#ifndef __OBJECTPROGRAMS_H__
#define __OBJECTPROGRAMS_H__

//abstract function meant to be called by each program with its own implementation
typedef int (*ObjectProgramRunner)(char *, char *, char *, char *);
//objput's function
int objects_objput(char *a, char *b, char *c, char *d);
//objget's function
int objects_objget(char *a, char *b, char *c, char *d);
//objlist's function
int objects_objlist(char *a, char *b, char *c, char *d);
//objgetacl's  function
int objects_objgetacl(char *a, char *b, char *c, char *d);
//objsetacl's  function
int objects_objsetacl(char *a, char *b, char *c, char *d);
//objtestacl's function
int objects_objtestacl(char *a, char *b, char *c, char *d);

#endif