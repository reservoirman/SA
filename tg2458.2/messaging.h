#ifndef __MESSAGING_H__
#define __MESSAGING_H__

//the long integer value for MessagingType
typedef enum {OBJPUT = 1004, OBJGET, OBJLIST, OBJGETACL, OBJSETACL, OBJTESTACL} MessageType;


//struct for sending messages
typedef struct {
	long int message_type;
	//char *message;//[0xFFFF];
	char user[0xFF];
	char group[0xFF];
	char object[0xFF];
	char content[0x1777];
} MessagingType;

typedef struct {
	long int message_type;
	char content[0x1A74];
} MessagingContentType;

//used by daemon to create the message queue
int messaging_Init();

//used by programs to send requests to daemon for processing
int messaging_sendMessage(MessageType type, char *u, char *g, char *o);

//used by programs to send actual content
int messaging_sendContent(char *c);

//used by daemon to process requests from programs
MessagingType * messaging_receiveMessage();



#endif