#include <signal.h>		//for signal
#include <stdio.h>		//for printf
#include <string.h>
#include <sys/msg.h>	//for msgset, msgrcv, msgsnd, IPC_CREAT
#include <errno.h>		//for errno
#include <stdlib.h>		//for EXIT_SUCCESS
#include "messaging.h"
#include "objectprograms.h"

//for indexing into the table of function pointers
typedef enum {
				NO_INDEX = 0,
				OBJPUT_INDEX, 
				OBJGET_INDEX,  
				OBJLIST_INDEX, 
				OBJGETACL_INDEX, 
				OBJSETACL_INDEX, 
				OBJTESTACL_INDEX
			} MessageTypeIndex;		

static ObjectProgramRunner _jump_table[OBJTESTACL_INDEX + 1];

#define QUEUE_ID 10
#define BUF_SIZE 100
#define FILE_SIZE 0xFFFFFF

static MessageTypeIndex _current_index = NO_INDEX;
static MessagingType _buffer;
static char _file[FILE_SIZE] = {0};
//to mark where we at filling in _file
static char *_marker;

int no_index_runner(char *a, char *b, char *c, char *d)
{
	printf("no_index_runner called!!!\n");
	return 0;
}

static void _setupJumpTable()
{
	_current_index = NO_INDEX;
	_jump_table[NO_INDEX] 			= no_index_runner;
	_jump_table[OBJPUT_INDEX] 		= objects_objput;
	_jump_table[OBJGET_INDEX] 		= objects_objget;
	_jump_table[OBJLIST_INDEX] 		= objects_objlist;
	_jump_table[OBJGETACL_INDEX] 	= objects_objgetacl;
	_jump_table[OBJSETACL_INDEX] 	= objects_objsetacl;
	_jump_table[OBJTESTACL_INDEX] 	= objects_objtestacl;
}

static void _closeMessageQueue()
{
	if (messaging_close() == 0)
	{
		printf("closed the message queue!  Angel exiting.\n");
		_jump_table[_current_index](NULL, NULL, NULL, NULL);
	}
	else
	{
		printf("Message queue failed to close! Angel exiting\n");
	}

	exit(EXIT_SUCCESS);
}

static void _callObjectProgram(MessagingType *out, MessageTypeIndex index)
{
	struct Request *r = &out->request;
	//reset file marker once we are calling on object program
	_marker = _file;
	//passing in the user, group, object, and content and call object program
	int result = _jump_table[index](r->user, r->group, r->object, _file);
	
	//obtain the result of that program and send back to the program
	messaging_sendFinished(result);
}

static void _buildingUpContent(MessagingType *out)
{
	memcpy(_marker, out->data.content, strlen(out->data.content));
		
	//if this is the eof, 
	if (out->data.eof == 1)
	{
		//reset file marker back to beginning of buffer
		_marker = _file;
		printf("THE CONTENT!!!! %s\n!!!!!!!!\n\n\n\n", _file);
	}
	//if this is not eof, keep accumulating data
	else
	{
		//accumulate more data
		_marker += strlen(out->data.content);
	}
}

//the one that runs as root
static void _messageDaemon()
{
	//run setsid to detach it from any particular session/terminal
	int set = setsid();
	if (set != -1)
	{
		umask(077);		//any files that the daemon creates, only the owner (root) can access them.
		
		//if we created the message queues successfully, start the daemon loop
		if (messaging_init() != -1)
		{
			printf("Message queues created! \n");
			//also, setup the jump table
			_setupJumpTable();
			//setup file buffer
			_marker = _file;
			memset(_file, 0, strlen(_file));
			//we loop as long as the message queues are available
			while (messaging_isAlive())
			{
				MessagingType *output = messaging_receiveRequest();
				if (output != NULL)
				{
					switch (output->message_type)
					{
						case OBJPUT:							
							_callObjectProgram(output, OBJPUT_INDEX);
							break;
						case OBJGET:
							_callObjectProgram(output, OBJGET_INDEX);
							break;
						case OBJLIST:
							_callObjectProgram(output, OBJLIST_INDEX);
							break;
						case OBJGETACL:
							_callObjectProgram(output, OBJGETACL_INDEX);
							break;
						case OBJSETACL:
							_callObjectProgram(output, OBJSETACL_INDEX);
							break;
						case OBJTESTACL:
							_callObjectProgram(output, OBJTESTACL_INDEX);
							break;
						case CONTENT:
							_buildingUpContent(output);
							break;
						default:	
							printf("ANGEL dump string: %s\n", (char *)output);
							if (strncmp((char *)output, "end", sizeof("end")) == 0)
							{
								_closeMessageQueue();
							}
							break;
					}
				}
			}		
		}
		else 
		{
			printf("Angel creation failed! Please delete message queues and try again\n");
			exit(EXIT_SUCCESS);
		}		
	}



}

int main()
{
	

	int result = fork();
	switch (result)
	{
		case -1:
			printf("%d: Starting daemon failed.  Please try again.\n", result);
			break;
		case 0:
			_messageDaemon();
			break;
		default:
			break;
	}

}