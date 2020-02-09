#include <windows.h>
#include <stdio.h>


void handle_last_error()
{

}


BOOL CALLBACK EnumDesktopProc(LPTSTR lpszDesktop,
                              LPARAM lParam)
{
  printf("Desktop : %s\n", lpszDesktop);
  char buff[10000];
  DWORD needed;
  
  
  HDESK hDesk = OpenDesktop(lpszDesktop, 0, TRUE, GENERIC_ALL);
  if (hDesk)
  {
    printf("Desktop Opened!!!\n");
    BOOL r = GetUserObjectInformation(hDesk, UOI_NAME, &buff, 10000, &needed); 
    if (r == TRUE)
    {
      printf("needed = %d\n", needed);
      printf("Blurk %s\n", buff);
    }
    else
    {
      printf("Failed %d\n", GetLastError());
    }
  }
  else
  {
  }

  return TRUE;
}

BOOL CALLBACK EnumWindowStationProc(LPTSTR lpszWindowStation,
                                    LPARAM lParam)
{
  printf("Station : %s\n", lpszWindowStation);

  HWINSTA hw = OpenWindowStationA(lpszWindowStation, TRUE, WINSTA_ALL_ACCESS);
  if (hw == NULL)
  {
    printf("ERROR\n");
    exit(1);
    // GetLastError
  }

  BOOL res = EnumDesktops(hw, EnumDesktopProc, 0);

  return TRUE;
}

int main(void)
{

  BOOL res = EnumWindowStations(EnumWindowStationProc, 0);
  


  
  return 0;
}
