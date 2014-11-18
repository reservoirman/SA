#ifndef __KEYSET_H__
#define __KEYSET_H__

//store the encrypted key (K2E)
int keyset_store(char *user, char *name, const char *K2E);

//retrieve the encrypted key
const char * keyset_retrieve(char *user, char *name);

//become the keymaker to run store/retrieve
void keyset_becomeKeyMaker();

//quit being the keymaker so you can no longer run store/retrieve
void keyset_quitKeyMaker();

#endif