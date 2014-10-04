#ifndef __ACLCHECKING_H__
#define __ACLCHECKING_H__



int aclchecking_isValid(char *user_name, char *group_name, char *acl, char *op, char *action, int surpress);
char * aclchecking_getACL();

#endif