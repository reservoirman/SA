#include "crypto.h"
#include "keyset.h"
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define LENGTH 16
#define IVSTRING "Lan-Kyoung Hong!"
static unsigned char _rBuff[LENGTH];\
static unsigned char blankFile[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};    

//use this as the IV
/* doesn't work: encryption and decryption cannot work off the same buffer

static unsigned char *_setIV(const unsigned char *k)
{
  static unsigned char _ivBuff[LENGTH];
  int i = 0;
  for (i = 0; i < LENGTH; i++)
  {
    _ivBuff[i] = k[LENGTH - (i + 1)];
  }
  printf("i is %d!!!\n", i);
  _ivBuff[LENGTH] = '\0';
  return _ivBuff;
}
*/
const unsigned char * objectcrypto_encrypt(char *user, char *object, char *content, char *passphrase)
{
  //the three keys
  const unsigned char *K1, *K2, *K2E, *IV, *Ci;
  
  //initialize
  crypto_init();

  //first, hash the passphrase.  This is K1
  K1 = crypto_MD5(passphrase);
  //printf("MD5 hash: ");
  //crypto_print(K1);

  //second, obtain the per-file urandom key.  This is K2
  int r = open("/dev/urandom", O_RDONLY);
  ssize_t result = read(r, _rBuff, LENGTH);
  if (result < 0)
  {
      // error, unable to read /dev/urandom
      printf("Error, unable to read /dev/urandom\n"); 
      K2 = "0123456789012345";
  
  }
  else
  {
      K2 = (const unsigned char *)_rBuff;
  }
  close(r);
  
  //printf("Encrypt K2: %s\n", K2); 

  //third, encrypt K2 using K1 as the key and an IV.  This is KE2
  IV = IVSTRING;//_setIV(K1);
  K2E = crypto_encryptAES(K1, IV, K2);


  //fourth, store KE2
  keyset_store(user, object, K2E);

  //fifth, round data to the nearest 16 bytes and encrypt content using K2
  if (strlen(content) == 0)
  {
    content = blankFile;  
  }
  Ci = crypto_encryptAES(K2, IV, (const unsigned char *)content);
  crypto_destroy();

  //return this content
  return Ci;
}

const unsigned char * objectcrypto_decrypt(char *user, char *object, char *content, char *passphrase)
{
  //the three keys
  const unsigned char *K1, *K2, *K2E, *IV, *De;
  
  //initialize
  crypto_init();

  //first, obtain K1 by hashing the passphrase.
  K1 = crypto_MD5(passphrase);

  //second, obtain K2E from the keySafe:
  K2E = keyset_retrieve(user, object);


  //third, use this to decrypt K2E to find K2.
  IV = IVSTRING;//_setIV(K1);
  K2 = crypto_decryptAES(K1, IV, K2E);
  //fourth, once you have K2, you can decrypt the content
  De = crypto_decryptAES(K2, IV, content);

  //check if the original file was blank 
  if (strncmp(De, blankFile, 16) == 0)
  {
    De = NULL;
  }

  crypto_destroy();

  //return this content
  return De;
}

/*
int main()
{
  const unsigned char *encrypted, *decrypted;
  char holla[10] = {1,2,3,4,5,6,7,8,9, 10};
  encrypted = objectcrypto_encrypt("ts", "holla", "The quick brown fox jumps over the lazy dog", "lkbeauty");
  crypto_print(encrypted);

  decrypted = objectcrypto_decrypt("ts", "holla", (unsigned char *)encrypted, "lkbeauty");
  crypto_print(decrypted);
  
}
*/