#ifndef __MESSAGING_H__
#define __MESSAGING_H__

//the long integer value for MessagingType
#define MESSAGE_TYPE	1004

//struct for sending messages
typedef struct {
	long int message_type;
	char *user;
	char *group;
	char *object;
	char *content;
} MessagingType;


//used by programs to send requests to daemon for processing
int messaging_sendMessage(MessagingType *message);

//used by daemon to process requests from programs
MessagingType * messaging_receiveMessage();



#endif