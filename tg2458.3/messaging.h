#ifndef __MESSAGING_H__
#define __MESSAGING_H__

//the long integer value for MessagingType
typedef enum {
				OBJPUT = 1004, 
				OBJGET, 
				OBJLIST, 
				OBJGETACL, 
				OBJSETACL, 
				OBJTESTACL,
				CONTENT,
				FINISHED
			} MessageType;

#define CHUNK_LENGTH 0x2F9

struct Request {
	char user[0xFF];
	char group[0xFF];
	char object[0xFF];
};

struct Data {
	char content[CHUNK_LENGTH];
	int eof;
};


//struct for sending messages
typedef struct {
	long int message_type;
	union {
		struct Request request;
		struct Data data;
	};
} MessagingType;

//struct for containing return code of programs
typedef struct {
	long int message_type;
	int return_code;
} MessagingFinishType;

//used by angel to create the message queues
int messaging_Init();

//used by programs to send requests to angel for processing
int messaging_sendRequest(MessageType type, char *u, char *g, char *o);

//used by programs to send actual content
int messaging_sendContent(char *c, int eof);

//used by the angel to let program know process request finished
int messaging_sendFinished(int return_code);

//used by angel to process requests and actual content from programs
MessagingType * messaging_receiveRequest();

//used by the program to receive the result of the process request
MessagingFinishType * messaging_receiveFinished();

//checks if message queues are still alive
int messaging_isAlive();

//closes the message queues
int messaging_close();
#endif