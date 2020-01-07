#include <stdio.h>
#include <string.h>


list_t *create_list()
{
  list_t *res = malloc(sizeof(list_t));


  return res;
}


void add_list(list_t *list, list_item_t *item)
{

}

list_item_t *find_list()
{
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
