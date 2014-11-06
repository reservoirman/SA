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
	//check if an ACL for this object already exists		
	char *acl = objects_readObject(u2, o, ACL);
	if (acl != NULL)
	{
		//if so, check the acl to see if this user is 
		//allowed to write to the file
		if (aclchecking_isValidOp(u1, g, acl, "w") == 0)
		{
			success = 1;
		}
	}
	//if the ACL for this object didn't exist before, 
	//create the ACL for it
	else
	{
		char *newacl = (char *)malloc(MAXNAMELENGTH + 20);
		sprintf(newacl, "%s.*\trwxpv\n", u1);
		if(objects_createObject(u2, o, newacl, ACL) == 0)
		{	
			free(newacl);
			success = 1;
			printf("OBJPUT: Created a new object ACL %s for user %s.\n", o, u2);
		}	
	}

	//finally, if all is well, create/update the object itself:
	if (success == 1)
	{
		if (objects_createObject(u2, o, c, DATA) == -1)
		{				
			printf("Failed to create object %s.  Please try again.\n", o);
		}
		else
		{
			success = 0;
			printf("OBJPUT: object written!\n");
		}
	}

	return success;
}


int objects_objget(char *u1, char *u2, char *g, char *o, char *c)
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
			printf("%s\n", contents);
			success = 0;
		}
		else
		{
			aclchecking_printErrno(u1, g, "r");
		}
	}
	else
	{
		printf("%s is not a valid object.  Please try again. \n", o);
	}

	return success;
}

int objects_objlist(char *u1, char *u2, char *g, char *o, char *c)
{
	int success = -1;
	int show_length = 0;
	char *length = "l";
	if (strstr(o, length) != NULL)
	{
		show_length = 1;
	}
	success = objects_listObjects(u1, show_length);
	if ( success == -1)
	{
		printf("%s is not a valid user.  Please try again.\n", u1);
	}

	return success;
}

int objects_objsetacl(char *u1, char *u2, char *g, char *o, char *c)
{
	int success = -1;

	//check if the ACL for the object exists and that the ACL message is good	
	char *acl = objects_readObject(u2, o, ACL);
	if (acl != NULL)
	{
		//check if the ACL content is valid:
		if (namechecking_check(c, ACLS) == 0)
		{
			//if so, check the acl to see if this user is 
			//allowed to write to the acl
			if (aclchecking_isValidOp(u1, g, acl, "p") == 0)
			{
				objects_createObject(u2, o, c, ACL);
			}
			else
			{
				aclchecking_printErrno(u1, g, "p");
			}			
		}
	}
	else
	{
		printf("%s is not a valid object.  Please try again. \n", o);
	}

	return success;
}

int objects_objgetacl(char *u1, char *u2, char *g, char *o, char *c)
{
	int success = -1;

	//check if this object exists		
	char *acl = objects_readObject(u2, o, ACL);
	//check if the ACL for this object is still good
	if (acl != NULL && namechecking_check(acl, ACLS) == 0)
	{
		//if so, check the acl to see if this user is 
		//allowed to read out the acl
		if (aclchecking_isValidOp(u1, g, acl, "v") == 0)
		{
			//read out the ACL and print it to the console
			char *theACL = aclchecking_getACL();
			if (theACL != NULL)
			{
				printf("%s\n", theACL);	
			}
			success = 0;

		}
		else
		{
			aclchecking_printErrno(u1, g, "v");
		}
	}
	else
	{
		printf("%s is not a valid object.  Please try again. \n", o);
	}

	return success;
}


int objects_objtestacl(char *u1, char *u2, char *g, char *o, char *op_name)
{
	//if the op code is valid:
	if (namechecking_check(op_name, OPS) == 0)
	{
		//check if this object exists		
		char *acl = objects_readObject(u2, o, ACL);

		//doublecheck that the content is good:
		if (acl != NULL && namechecking_check(acl, ACLS) == 0)
		{
			if (aclchecking_isValidOp(u1, g, acl, op_name) == -1)
			{
				printf("denied\n");
			}
			else
			{
				printf("allowed\n");
			}
		}
		else
		{
			printf("%s is not a valid object.  Please try again. \n", o);
		}

	}
}

