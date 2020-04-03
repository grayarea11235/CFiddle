#include <windows.h>
#include <stdio.h>


/* Private Function GetSysLVHwnd() As Long */
/*    Dim h As Long */
/*    h = FindWindow("Progman", vbNullString) */
/*    h = FindWindowEx(h, 0, "SHELLDLL_defVIEW", vbNullString) */
/*    GetSysLVHwnd = FindWindowEx(h, 0, "SysListView32", vbNullString) */
/* End Function */

BOOL CALLBACK FindDLV( HWND hWndPM, LPARAM lParam )
{

   HWND hWnd = FindWindowEx( hWndPM, NULL, _T("SHELLDLL_DefView"), NULL );
   if (hWnd)
   {
      // This is our window!
      HWND* phWnd = (HWND*)lParam;
      *phWnd = hWnd;

      return FALSE; // all done
   }

   return TRUE; // keep looking
}


HWND GetDesktopListView()
{
   HWND hWndPM = FindWindowEx(NULL, NULL, _T("Progman"), NULL);
   if (!hWndPM)
	return NULL;

   HWND hWnd = FindWindowEx( hWndPM, NULL, _T("SHELLDLL_DefView"), NULL );
   if (!hWnd)
   {
      // Under Win2K this seems to move around, and there can be many WorkerW windows
      EnumWindows( FindDLV, LPARAM((HWND*)&hWnd) );
      if (!hWnd)
         return NULL;
   }

   HWND hWndLV = FindWindowEx( hWnd, NULL, _T("SysListView32"), NULL );

   return hWndLV;
}

HWND get_desktop_lv_hwnd()
{
  HWND h;

  h = FindWindow("Progman", NULL);
  printf("1. h = 0x%08X\n", h);
  
  h = FindWindowEx(h, 0, "SHELLDLL_defVIEW", NULL);
  printf("2. h = 0x%08X\n", h);

  h = FindWindowEx(h, 0, "SysListView32", NULL);
  printf("3. h = 0x%08X\n", h);

  return h;
}


int main(void)
{
  HWND lv_hwnd = get_desktop_lv_hwnd();

  printf("HWND = 0x%08X\n", lv_hwnd);
  
  return 0;
}



 
