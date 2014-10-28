#include <stdlib.h>		//for malloc
#include <stdio.h>		//for printf
#include "linkedlist.h"

int main()
{
	char *a = "holla\n", *b = "back\n", *c = "youngin\n", *d = "woop woop\n",
	*e = "TSG!\n";
	Item *the_list = linkedlist_newList((void *)d);

	Item *traverser = the_list;
	linkedlist_insertItem(traverser, (void *)a);
	linkedlist_insertItem(traverser, (void *)b);
	linkedlist_insertItem(traverser, (void *)c);
	linkedlist_insertItem(traverser, (void *)d);
	linkedlist_updateItem(traverser, (void *)c, (void*)e);

	while (the_list != 0)
	{
		printf("%s", (char *)the_list->item);
		the_list = the_list->next;
	}
	linkedlist_clearList(traverser);
	printf("cleared the list\n");
	while (the_list != 0)
	{
		printf("%s", (char *)the_list->item);
		the_list = the_list->next;
	}


}