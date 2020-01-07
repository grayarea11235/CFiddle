tyedef struct _list_item
{
  void *data;
  long size;
  
  list_item_t *next;
} list_item_t;


typedef struct _list_t
{

  list_item_t *head;

  
} list_t;
