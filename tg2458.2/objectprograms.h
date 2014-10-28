#ifndef __OBJECTPROGRAMS_H__
#define __OBJECTPROGRAMS_H__

//abstract function meant to be called by each program with its own implementation
typedef int (*ObjectProgramRunner)(char *, char *, char *, char *, char *);
//objput's function
int objects_objput(char *a, char *b, char *c, char *d, char *e);
//objget's function
int objects_objget(char *a, char *b, char *c, char *d, char *e);
//objlist's function
int objects_objlist(char *a, char *b, char *c, char *d, char *e);
//objgetacl's  function
int objects_objgetacl(char *a, char *b, char *c, char *d, char *e);
//objsetacl's  function
int objects_objsetacl(char *a, char *b, char *c, char *d, char *e);
//objtestacl's function
int objects_objtestacl(char *a, char *b, char *c, char *d, char *e);

#endif