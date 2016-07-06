#pragma once

namespace toni
{
  bool InjectDll(DWORD processId, const std::string& dllPath);
}