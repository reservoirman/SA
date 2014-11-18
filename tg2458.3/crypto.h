#ifndef __CRYPTO_H__
#define __CRYPTO_H__

int crypto_init();
const unsigned char *  crypto_MD5(char *passphrase);
const unsigned char *  crypto_encryptAES(const char *key, const char *IV, const char *plaintext);
const unsigned char *  crypto_decryptAES(const char *key, const char *IV, const char *ciphertext);
int crypto_destroy();
int crypto_print(const unsigned char *output);

#endif