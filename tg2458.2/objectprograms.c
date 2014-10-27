#include <stdio.h>
#include <string.h>
#include <unistd.h>			//for getuid
#include <stdlib.h>
#include <sys/types.h>		//for uid_t and gid_t
#include <pwd.h>			//for struct passwd
#include <grp.h>			//for struct grp
#include "namechecking.h"
#include "objects.h"
#include "objectprograms.h"
#include "messaging.h"
#include "aclchecking.h"
#include <sys/msg.h>

static char buffer[OBJECT_SIZE];

static MessagingType _objput_message;

int objects_objput(char *u1, char *u2, char *g, char *o, char *c)
{
	int success = -1;
	
	//check if this object already exists		
	char *acl = objects_readObject(u2, o, ACL);
	if (acl != NULL)
	{
		//if so, check the acl to see if this user is 
		//allowed to read from the file
		if (aclchecking_isValidOp(u1, g, acl, "r") == 0)
		{
			//if we do have permission, let's go ahead and read the file
			char *contents = objects_readObject(u2, o, DATA);
			if (contents != NULL)
			printf("%s", contents);
			success = 0;
		}
		else
		{
			aclchecking_printErrno(u1, g, "r\0");
		}
	}
	else
	{
		printf("%s is not a valid object.  Please try again. \n", o);
	}

	return success;
}

int objects_objget(char *u1, char *u2, char *g, char *o, char *c)
{

}

int objects_objlist(char *u1, char *u2, char *g, char *o, char *c)
{
	
}

int objects_objsetacl(char *u1, char *u2, char *g, char *o, char *c)
{
	
}

int objects_objgetacl(char *u1, char *u2, char *g, char *o, char *c)
{
	
}

int objects_objtestacl(char *u1, char *u2, char *g, char *o, char *c)
{
	
}

