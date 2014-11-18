#ifndef __OBJECTCRYPTO_H__
#define __OBJECTCRYPTO_H__

int crypto_init();
const unsigned char * objectcrypto_encrypt(char *user, char *object, char *content, char *passphrase);
const unsigned char * objectcrypto_decrypt(char *user, char *object, char *content, char *passphrase);



#endif