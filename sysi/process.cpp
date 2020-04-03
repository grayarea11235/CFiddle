#include <windows.h>
#include <psapi.h>
#include <stdio.h>

void print_proc_info(DWORD proc_id)
{
  TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
  
  HANDLE proc_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                  FALSE, proc_id);
  
  // Get the process name.
  if(NULL != proc_handle)
  {
    HMODULE hMod;
    DWORD cbNeeded;
    
    if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), 
                          &cbNeeded))
    {
      GetModuleBaseName(hProcess, hMod, szProcessName, 
                        sizeof(szProcessName)/sizeof(TCHAR));
    }
  }
  
  // Print the process name and identifier.
  
  _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );
  
  // Release the handle to the process.
  
  CloseHandle(hProcess);
}


int get_procs()
{
  DWORD required = 0;
  DWORD procs[10000];
  DWORD processes = 0;
  
//  BOOL res = EnumProcesses(DWORD   *lpidProcess, DWORD   cb, LPDWORD lpcbNeeded);
  if (EnumProcesses(procs, sizeof(DWORD) * 10000, &required) == TRUE)
  {
    printf("Enum OK required is %d\n", required);
  }

  processes = required / sizeof(DWORD);
  
  for(int i = 0; i < processes; ++i)
  {
    if(processes[i] != 0 )
    {
      PrintProcessNameAndID( aProcesses[i] );
    }
  }
  
  return required;
}

int main(void)
{
  get_procs();

  return 0;
}
