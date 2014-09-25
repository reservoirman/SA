#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

static Item *newItem;

Item * linkedlist_newList(void *object)
{
	Item *newList = (Item *)malloc(sizeof(Item));
	newList->item = object;
	newList->next = 0;

	return newList;
}

void linkedlist_insertItem(Item *list, void *object)
{
	newItem = (Item *)malloc(sizeof(Item));
	newItem->item = object;
	newItem->next = 0;
	int i =1;
	while (list->next != 0)
	{
		printf("%d!!", i);
		list = list->next;
		i++;
	}
	printf("\n");
	list->next = newItem;

}

void linkedlist_updateItem(Item *list, void *currentItem, void *updatedItem)
{
	while (list != 0)
	{
		if (strcmp((char *)list->item, (char *)currentItem) == 0)
		{
			list->item = updatedItem;
			break;
		}
		list = list->next;
	}
}

void linkedlist_updateItems(Item *list, void *currentItem, void *updatedItem)
{
	while (list != 0)
	{
		if (strcmp((char *)list->item, (char *)currentItem) == 0)
		{
			list->item = updatedItem;
		}
		list = list->next;
	}
}

void linkedlist_clearList(Item *list)
{
	Item *currentItem;
	while (list != 0)
	{
		currentItem = list;
		list = list->next;
		free(currentItem);
	}
}


