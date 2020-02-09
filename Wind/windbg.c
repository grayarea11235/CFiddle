#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <time.h>
#include <stdio.h>

#define TIMEOUT_WIN_DEBUG 100

struct dbwin_buffer
{
  DWORD   dwProcessId;
  char    data[4096 - sizeof(DWORD)];
};

struct mutex_info
{
  HANDLE DBWinMutex;
  HANDLE DBMonBuffer;
  HANDLE EventBufferReady;
  HANDLE EventDataReady;
  struct dbwin_buffer *m_pDBBuffer;
};

DWORD deinit_mutex(struct mutext_info *m_info);


DWORD deinit_mutex(struct mutext_info *m_info)
{
  
  if (m_info->DBWinMutex != NULL)
  {
    CloseHandle(m_info->DBWinMutex);
    m_info->DBWinMutex = NULL;
  }
  
  if (m_info->DBMonBuffer != NULL)
  {
    UnmapViewOfFile(m_info->DBBuffer);
    CloseHandle(m_info->DBMonBuffer);
    m_info->DBMonBuffer = NULL;
  }
  
  if (m_info->EventBufferReady != NULL)
  {
    CloseHandle(m_info->EventBufferReady);
    m_info->EventBufferReady = NULL;
  }
  
  if (m_info->EventDataReady != NULL)
  {
    CloseHandle(m_info->EventDataReady);
    m_info->EventDataReady = NULL;
  }
  
  m_info->DBBuffer = NULL;
    
  return 0;
}

DWORD init_mutex(struct mutex_info *m_info)
{
  DWORD errorCode = 0;
  BOOL bSuccessful = FALSE;

  SetLastError(0);


  CComBSTR sDBWinMutex = L"DBWinMutex";
  m_info->DBWinMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, sDBWinMutex);

  if (m_info->DBWinMutex == NULL) 
  {
    errorCode = GetLastError();
    printf("Error 1 : LastError is %d\n", errorCode);
    return errorCode;
  }
	
  // Event: buffer ready
  // ---------------------------------------------------------
  CComBSTR DBWIN_BUFFER_READY = L"DBWIN_BUFFER_READY";
  m_hEventBufferReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, DBWIN_BUFFER_READY);

  if (m_hEventBufferReady == NULL) 
  {
    m_hEventBufferReady = CreateEvent(NULL,
                                      FALSE,    // auto-reset
                                      TRUE,    // initial state: signaled
                                      DBWIN_BUFFER_READY);
    
    if (m_hEventBufferReady == NULL)
    {
      errorCode = GetLastError();
      printf("Error 2 : LastError is %d\n", errorCode);
      return errorCode;
    }
  }

  // Event: data ready
  // ---------------------------------------------------------
  CComBSTR DBWIN_DATA_READY = L"DBWIN_DATA_READY";
  m_hEventDataReady = OpenEvent(SYNCHRONIZE,
                                FALSE,
                                DBWIN_DATA_READY);
  
  if (m_hEventDataReady == NULL) {
    m_hEventDataReady = CreateEvent(NULL,
                                    FALSE,    // auto-reset
                                    FALSE,    // initial state: nonsignaled
                                    DBWIN_DATA_READY);

    if (m_hEventDataReady == NULL)
    {
      errorCode = GetLastError();
      printf("Error 3 : LastError is %d\n", errorCode);
      return errorCode;
    }
  }

  // Shared memory
  // ---------------------------------------------------------
  CComBSTR DBWIN_BUFFER = L"DBWIN_BUFFER";
  m_hDBMonBuffer = OpenFileMapping(
    FILE_MAP_READ,
    FALSE,
    DBWIN_BUFFER
    );

  if (m_hDBMonBuffer == NULL) {
    m_hDBMonBuffer = CreateFileMapping(
      INVALID_HANDLE_VALUE,
      NULL,
      PAGE_READWRITE,
      0,
      sizeof(struct dbwin_buffer),
      DBWIN_BUFFER
      );

    if (m_hDBMonBuffer == NULL) {
      errorCode = GetLastError();
      printf("Error 4 : LastError is %d\n", errorCode);
      return errorCode;
    }
  }

  m_pDBBuffer = (struct dbwin_buffer *)MapViewOfFile(m_hDBMonBuffer,
                                                     SECTION_MAP_READ,
                                                     0,
                                                     0,
                                                     0);
  
  if (m_pDBBuffer == NULL)
  {
    errorCode = GetLastError();
    printf("Error 5 : LastError is %d\n", errorCode);
    return errorCode;
  }

  DWORD ret = 0;
	
  // Main check loop
  while (1)
  {
    // wait for data ready
    //printf("About to wait\n");
    ret = WaitForSingleObject(m_hEventDataReady, TIMEOUT_WIN_DEBUG);
    //printf("Wait ended. ret = %d\n", ret);

    if (ret == WAIT_ABANDONED)
    {
      //printf("WAIT_ABANDONED\n");
    }
    if (ret == WAIT_TIMEOUT)
    {
      //printf("WAIT_TIMEOUT\n");
    }

    if (ret == WAIT_FAILED)
    {
      errorCode = GetLastError();
      printf("Error 6 : LastError is %d\n", errorCode);
      return errorCode;
    }

    if (ret == WAIT_OBJECT_0) {
      //printf("GOT IT! : ");
      time_t ltime; /* calendar time */
      ltime = time(NULL); /* get current cal time */

      time_t rawtime;
      struct tm *info;
      char buffer[80];
//			char buffer[26];

      time(&rawtime);

      info = localtime(&rawtime);

//			strftime(buffer, 80, "%x - %I:%M%p", info);
      strftime(buffer, 80, "%Y:%m:%d %H:%M:%S", info);

      printf("%s : (%d) %s", buffer, m_pDBBuffer->dwProcessId, m_pDBBuffer->data);

      // signal buffer ready
      SetEvent(m_hEventBufferReady);
    }
  }

  return 0;
}

int main()
{
  struct mutex_info m_info;
  init_mutex(&m_info);
  
  printf("Done\n");
  getchar();

  return 0;
}
