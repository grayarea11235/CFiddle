#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct _list_item
{
  void *data;
  long size;
  
  struct _list_item_t *next;
} list_item_t;


typedef struct _list_t
{
  list_item_t *head;
  list_item_t *tail;
} list_t;

list_t *create_list();
int get_count(list_t *list);
void add_to_list(list_t *list, void *data, int size);
list_item_t *get_list_index(list_t *list, int index);
void destroy_list(list_t *list);

#endif
