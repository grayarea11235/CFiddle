#include <windows.h>


void init_mutex()
{
  CComBSTR DBWinMutex = L"DBWinMutex";
  m_hDBWinMutex = ::OpenMutex(
                              MUTEX_ALL_ACCESS,
                              FALSE,
                              DBWinMutex
                              );
  
  if (m_hDBWinMutex == NULL) {
    errorCode = GetLastError();
    return errorCode;
  }
  

}

int main(void)
{

}
