#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"



Item * linkedlist_newList(void *object, size_t size)
{
	Item *newList = (Item *)malloc(sizeof(Item));
	newList->item = malloc(size);
	strcpy(newList->item, object);
	newList->next = NULL;

	return newList;
}

void linkedlist_insertItem(Item *list, void *object, size_t size)
{
	Item *newItem = (Item *)malloc(sizeof(Item));
	newItem->item = malloc(size);
	strcpy(newItem->item, object);
	newItem->next = NULL;
	int i = 1;
	while (list->next != NULL)
	{
		list = list->next;
		i++;
	}
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

int linkedlist_searchItem(Item *list, void *currentItem)
{
	int success = -1;
	while (list != 0)
	{
		if (strcmp((char *)list->item, (char *)currentItem) == 0)
		{
			success = 0;
			break;
		}
		list = list->next;
	}
	return success;
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


