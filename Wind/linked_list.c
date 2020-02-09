#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linked_list.h"


// Find func pointer
int (*find_func)(void *item, void *);

list_t *create_list()
{
  list_t *res = malloc(sizeof(list_t));

  res->head = NULL;
  res->tail = NULL;

  return res;
}

int get_count(list_t *list)
{
  list_item_t *curr = list->head;
  int c = 0;
  
  while(curr != NULL)
  {
    curr = (list_item_t *)curr->next;
    c++;
  }

  return c;
}

// returns NULL if list empty
list_item_t *get_last_item(list_t *list)
{
  list_item_t *curr = list->head;

  while(curr != NULL)
  {
    curr = (list_item_t *)curr->next;
  }

  return curr;
}


void add_to_list(list_t *list, void *data, int size)
{
  list_item_t *new_item = malloc(sizeof(list_item_t)); 
  new_item->data = data;
  new_item->size = size;
  new_item->next = NULL;
  
  if (list->head == NULL)
  {
    list->head = new_item;
    list->tail = new_item;
  }
  else
  {
    if (list->tail != NULL)
    {
      list->tail->next = (void *)new_item;
      list->tail = new_item;
    }
    else
    {
      // Weird error shouldnt ever happen
    }
  }
}


list_item_t *find_in_list(list_t *list)
{
  
  
}


list_item_t *get_list_index(list_t *list, int index)
{
  list_item_t *ret = list->head;
  int c = 0;

  if (list->head != NULL)
  {
    while ((c < index) || (ret->next != NULL))
    {
      c++;
    }
  }
  
  return ret;
}

void destroy_list(list_t *list)
{

}


#ifdef TEST

int main(void)
{
  list_t *list = create_list();
  list_item_t *item = malloc(sizeof(list_item_t));

  item->data = malloc(sizeof(char) * 256);
  strcpy(data, "Some test data");
  item->size = strlen(item->data);
}


#endif
