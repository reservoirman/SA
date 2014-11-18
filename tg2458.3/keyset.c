#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "keyset.h"

static const char *path = "/home/w4118/Documents/Columbia/CS 4187/tg2458.3/keySafe";
static const char *keymaker = "keymaker";
static uid_t keymakerID = 1007, original;
static FILE *file;
static char keyBuffer[256], pwd[1024];

static char * _constructKeyFileName(char *user, char *name)
{
	int filenamelength = strlen(user) + strlen(name);
	char *filename = (char *)malloc(filenamelength);
	//constructing the filename
	strcpy(filename, user);
	strcpy(filename + strlen(user), name);

	return filename;
}

static int _enterKeySafe()
{
	getcwd(pwd, 1024);
	char *keysafe;
	char hostname[256];
	gethostname(hostname, 256);		
	if (strcmp(hostname, "ubuntu") == 0)
	{
		keysafe = "/home/w4118/Documents/Columbia/CS 4187/tg2458.3/keySafe/";
	}
	//otherwise we are running on the class VM and use this path:
	else keysafe = "/home/tinyvm/tg2458.3/keySafe/";
	return chdir(keysafe);
}

static void _exitKeySafe()
{
	chdir(pwd);
}

int keyset_store(char *user, char *name, const char *K2E)
{
	int success = -1;
	if (keymakerID != getuid())
	{
		printf("DANGER: intruder attempting to access the key safe!!!  Terminating\n");
		exit(1);
	}	
	else 
	{
		if (_enterKeySafe() == 0)
		{
			char *filename = _constructKeyFileName(user, name);
			int file = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
			if (file > 0)
			{
				size_t bytes_written = write(file, (void *)K2E, strlen(K2E));
				if (bytes_written == strlen(K2E))
				{
					success = 0;
				}
				close(file);
			}		
			_exitKeySafe();
		}
	}
	return success;
}

const char * keyset_retrieve(char *user, char *name)
{
	if (keymakerID != getuid())
	{
		printf("DANGER: intruder attempting to access the key safe!!!  Terminating\n");
		exit(1);
	}	
	else 
	{
		if (_enterKeySafe() == 0)
		{
			char *filename = _constructKeyFileName(user, name);
			int file = open(filename, O_RDONLY);
			if (file > 0)
			{
				size_t bytes_read = read(file, (void *)keyBuffer, 256);
				close(file);
			}		
			_exitKeySafe();
		}		
	}	

	return (const char *)keyBuffer;
}

void keyset_becomeKeyMaker()
{
	struct passwd *k = getpwnam(keymaker);
	keymakerID = k->pw_uid;
	original = geteuid();
	setreuid(keymakerID, original);
}

void keyset_quitKeyMaker()
{
	setreuid(original, original);
}


//the call to store the encrypted random file-encrypting key
//note: only keymaker can call this!
/*
int main (int argc, char **argv)
{
	keyset_becomeKeyMaker();
	keyset_store("ts", "holla", "damn");
	printf("The key is : %s\n", keyset_retrieve("ts", "holla"));
	keyset_quitKeyMaker();
	printf("Current id = %d, keymaker id = %d!!\n", geteuid(), keymakerID);

}
*/
