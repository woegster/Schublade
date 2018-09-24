#pragma once
#include <wtypes.h>
#include <string>

namespace toni
{
  bool InjectDll(DWORD processId, const std::string& dllPath);
}