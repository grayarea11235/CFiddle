#include <stdio.h>

void xor_swap(int *x, int *y)
{
  *x = *x ^ *y;
  *y = *x ^ *y;
  *x = *x ^ *y;
}

int main(void)
{
  int x = 15;
  int y = 7;

  printf("x = %d y =%d\n", x, y);
  xor_swap(&x, &y);
  printf("x = %d y =%d\n", x, y);

  return 0;
}
