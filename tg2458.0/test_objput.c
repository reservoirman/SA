#include <stdlib.h>		//for malloc
#include <stdio.h>		//for printf
#include "linkedlist.h"

int main()
{
	//test out whether directly assigning pointer to pointer for user and name is ok,
	//across multiple objects.
	int result = 0;

	result |= objects_createObject("ts", "holla", "what is going on?");
	result |= objects_createObject("lk", "holla", "I dunno");
	result |= objects_createObject("lk", "gangsta", "I dunno");
	result |= objects_createObject("ts", "gangsta2", "time to party");
	result ^= objects_createObject("tu", "didthiswork", "time to party");
	result |= objects_listObjects("ts", 0);
	result |= objects_listObjects("lk", 0);
	result ^= objects_listObjects("tu", 0);

	printf("\n\n");

	if (result == 0)
	{
		printf("objput tests passed!\n");
	}
	else
	{
		printf("objput tests failed.\n");
	}
	fprintf(stderr, "Come back home\n");

	return 0;
}