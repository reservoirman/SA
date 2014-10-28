#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

struct Item1
{
	void *item;
	struct Item1 *next;	
};                   

typedef struct Item1 Item;

Item * linkedlist_newList(void *object, size_t size);
void linkedlist_insertItem(Item *list, void *item, size_t size);
void linkedlist_updateItem(Item *list, void *currentItem, void *updatedItem);
void linkedlist_updateItems(Item *list, void *currentItem, void *updatedItem);
int linkedlist_searchItem(Item *list, void *currentItem);
void linkedlist_clearList(Item *list);


#endif