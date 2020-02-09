#include <windows.h>
#include <stdio.h>

int main(void)
{
  char output_str[4096];
  int i = 0;
  
  while(1) {
    sprintf(output_str, "Output debug from %d number %d\n", GetCurrentProcessId(), ++i);
    OutputDebugString(output_str);

    Sleep(1000);
  }
}
