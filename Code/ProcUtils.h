#pragma once
#include <string>
#include <vector>
#include <functional>

namespace toni
{
  void ForEachProcess(std::function<void(DWORD id)> action);
  void ForEachModule(HANDLE process, std::function<void(HMODULE module)> action);

  std::vector<DWORD> FindProcessesWhereModuleIsLoaded(const std::string& procName);
}