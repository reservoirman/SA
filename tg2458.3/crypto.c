#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "crypto.h"
#include "objects.h"

static int md_len = 0;
static EVP_MD_CTX *mdctx;
static const EVP_MD *md;
static unsigned char _hash[EVP_MAX_MD_SIZE];
static unsigned char _encryptedtext[OBJECT_SIZE];
static unsigned char _decryptedtext[OBJECT_SIZE];

static EVP_CIPHER_CTX *ctx;

static void handleErrors(void)
{
  ERR_print_errors_fp(stderr);

  abort();
}

//initialize all the structures
int crypto_init()
{

	OpenSSL_add_all_digests();
	//using md5
	md = EVP_md5();	
	mdctx = EVP_MD_CTX_create();
	


	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

}
const unsigned char * crypto_MD5(char *passphrase)
{
	//unsigned char md_value[EVP_MAX_MD_SIZE];
	int i;
	//NULL engine will use the default implementation for MD5
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, passphrase, strlen(passphrase));
	EVP_DigestFinal_ex(mdctx, _hash, &md_len);
/*	debugging code
	printf("Passphrase is: %s \n", passphrase);
	printf("md_len = %d\n", md_len);
	for(i = 0; i < md_len; i++)
	    printf("%02x", _hash[i]);
	printf("\n");
*/
	return (const unsigned char *)_hash;
}

const unsigned char * crypto_encryptAES(const char *key, const char *IV, const char *plaintext)
{
	int encryptedtext_len, len;
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
  	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, IV))
    handleErrors();
  	if(1 != EVP_EncryptUpdate(ctx, _encryptedtext, &len, plaintext, strlen(plaintext)))
    handleErrors();
  	encryptedtext_len = len;
	if(1 != EVP_EncryptFinal_ex(ctx, _encryptedtext + len, &len)) handleErrors();
	encryptedtext_len += len;

	_encryptedtext[encryptedtext_len] = '\0';

	return (const unsigned char*)_encryptedtext;
}

const unsigned char * crypto_decryptAES(const char *key, const char *IV, const char *ciphertext)
{
	int len, decryptedtext_len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
	* and IV size appropriate for your cipher
	* In this example we are using 256 bit AES (i.e. a 256 bit key). The
	* IV size for *most* modes is the same as the block size. For AES this
	* is 128 bits */
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, IV))
	handleErrors();
	/* Provide the message to be decrypted, and obtain the plaintext output.
	* EVP_DecryptUpdate can be called multiple times if necessary
	*/
	if(1 != EVP_DecryptUpdate(ctx, _decryptedtext, &len, ciphertext, strlen(ciphertext)))
	handleErrors();
	decryptedtext_len = len;
	/* Finalise the decryption. Further plaintext bytes may be written at
	* this stage.
	*/
	if(1 != EVP_DecryptFinal_ex(ctx, _decryptedtext + len, &len)) handleErrors();
	decryptedtext_len += len;
	_decryptedtext[decryptedtext_len] = '\0';
	return (const unsigned char *)_decryptedtext;
}

int crypto_destroy()
{
	EVP_CIPHER_CTX_free(ctx);

	/* Clean up */
	EVP_cleanup();
	ERR_free_strings();
}

int crypto_print(const unsigned char *output)
{
	int n = 0;
	if (output == _hash)
	{
		int i;
		for(i = 0; i < strlen(output); i++)
		n += printf("%02x", output[i]);
		n += printf("\n");
	}
	else if (output == _encryptedtext)
	{
		n = BIO_dump_fp(stdout, output, strlen(output));
	}
	else
	{
		n = printf("%s\n", output);
	}
	return n;
}
