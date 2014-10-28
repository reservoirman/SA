#ifndef __ACLCHECKING_H__
#define __ACLCHECKING_H__

typedef enum {
	ACL_SUCCESS = 0,
	ACL_USERCANNOT = -1, 
	ACL_GROUPCANNOT = -2, 
	ACL_USERINVALID = -3, 
	ACL_GROUPINVALID = -4 } ACLERRNO;

void aclchecking_printErrno(char *user_name, char *group_name, char *op);
int aclchecking_isValidOp(char *user_name, char *group_name, char *acl, char *op);
char * aclchecking_getACL();

#endif