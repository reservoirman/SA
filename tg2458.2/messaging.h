#ifndef __MESSAGING_H__
#define __MESSAGING_H__

//used by programs to send requests to daemon for processing
int messaging_sendMessage(char *message, size_t length);

//used by daemon to process requests from programs
char * messaging_receiveMessage(size_t length);



#endif