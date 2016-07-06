#include "stdafx.h"
#include "ProcUtils.h"
#include <Windows.h>
#include <psapi.h>

namespace toni
{
  void ForEachProcess(std::function<void(DWORD id)> action)
  {
    DWORD processIDs[1024];
    DWORD cbNeeded;
    DWORD processCount;

    std::vector<DWORD> matchingProcesses;

    if (EnumProcesses(processIDs, sizeof(processIDs), &cbNeeded))
    {
      processCount = cbNeeded / sizeof(DWORD);
      for (DWORD i = 0; i < processCount; i++)
      {
        if (processIDs[i] != 0)
        {
          action(processIDs[i]);
        }
      }
    }
  }

  void ForEachModule(HANDLE process, std::function<void(HMODULE module)> action)
  {
    HMODULE modules[256] = { 0 };
    DWORD needed = 0;
    if (EnumProcessModules(process, modules, sizeof(modules), &needed))
    {
      for (size_t i = 0; i < (needed / sizeof(HMODULE)); i++)
      {
        action(modules[i]);
      }
    }
  }

  std::vector<DWORD> FindProcessesWhereModuleIsLoaded(const std::string& modName)
  {
    std::vector<DWORD> matchingProcesses;

    ForEachProcess([&](DWORD procId)
    {
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procId);
      if (hProcess != INVALID_HANDLE_VALUE)
      {
        ForEachModule(hProcess, [&](HMODULE module)
        {
          char szModuleName[MAX_PATH] = "";
          GetModuleBaseNameA(hProcess, module, szModuleName, sizeof(szModuleName));
          if (modName.compare(szModuleName) == 0)
            return matchingProcesses.push_back(procId);
        });
        CloseHandle(hProcess);
      }      
    });

    return matchingProcesses;
  }
}