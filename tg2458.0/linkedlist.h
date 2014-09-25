#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

struct Item1
{
	void *item;
	struct Item1 *next;	
};                   

typedef struct Item1 Item;

Item * linkedlist_newList(void *object);
void linkedlist_insertItem(Item *list, void *item);
void linkedlist_updateItem(Item *list, void *currentItem, void *updatedItem);
void linkedlist_updateItems(Item *list, void *currentItem, void *updatedItem);
void linkedlist_clearList(Item *list);


#endif