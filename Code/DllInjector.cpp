#include <Windows.h>
#include "DllInjector.h"

namespace toni
{
  void GiveSelfDebugPriviledge()
  {
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
      TOKEN_PRIVILEGES tkp;
      LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
      CloseHandle(hToken);
    }
  }

  class RemoteMemoryAllocator
  {
  public:
    RemoteMemoryAllocator(HANDLE Process)
      : m_process(Process)
    {

    }

    bool Allocate(size_t bytes)
    {
      m_memory = VirtualAllocEx(m_process, NULL, bytes, MEM_COMMIT, PAGE_READWRITE);
      m_allocatedSize = bytes;
      return (m_memory != nullptr);
    }

    bool Write(const void* data, size_t bytes)
    {
      SIZE_T written = 0;
      if (WriteProcessMemory(m_process, m_memory, data, bytes, &written) == TRUE)
      {
        return written == bytes;
      }

      return false;
    }

    bool AllocateAndWrite(const void* data, size_t size)
    {
      if (Allocate(size))
        return Write(data, size);

      return false;
    }

    operator void*()
    {
      return m_memory;
    }

    ~RemoteMemoryAllocator()
    {
      if (m_memory != nullptr)
      {
        VirtualFreeEx(m_process, m_memory, m_allocatedSize, MEM_RELEASE);
      }
    }
  private:
    HANDLE m_process;
    void* m_memory = nullptr;
    size_t m_allocatedSize = 0;
  };

  class RemoteThread
  {
  public:
    ~RemoteThread()
    {
      if (m_remoteThread != INVALID_HANDLE_VALUE)
      {
        CloseHandle(m_remoteThread);
        m_remoteThread = INVALID_HANDLE_VALUE;
      }
    }

    bool Create(HANDLE process, LPTHREAD_START_ROUTINE startAdr, void* parameter)
    {
      m_remoteThread = CreateRemoteThread(process, NULL, 0, startAdr, parameter, 0, NULL);
      return m_remoteThread != nullptr;
    }

    void WaitForFinish()
    {
      WaitForSingleObject(m_remoteThread, INFINITE);
    }

    DWORD GetExitCode()
    {
      DWORD exitCode = 0;
      GetExitCodeThread(m_remoteThread, &exitCode);
      return exitCode;
    }
  private:
    HANDLE m_remoteThread = INVALID_HANDLE_VALUE;
  };

  class ProcessHandle
  {
  public:
    bool OpenById(DWORD procId)
    {
      m_proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
      return m_proc != INVALID_HANDLE_VALUE;
    }

    ~ProcessHandle()
    {
      if (m_proc != INVALID_HANDLE_VALUE)
      {
        CloseHandle(m_proc);
        m_proc = INVALID_HANDLE_VALUE;
      }
    }

    operator HANDLE()
    {
      return m_proc;
    }
  private:
    HANDLE m_proc = INVALID_HANDLE_VALUE;
  };

  bool InjectDll(DWORD processId, const std::string& dllPath)
  {
    GiveSelfDebugPriviledge();

    HMODULE hLocKernel32 = GetModuleHandleW(L"KERNEL32");
    if (!hLocKernel32)
      return false;

    FARPROC hLocLoadLibrary = GetProcAddress(hLocKernel32, "LoadLibraryA");
    if (!hLocLoadLibrary)
      return false;

    ProcessHandle remoteProcess;
    if (!remoteProcess.OpenById(processId))
      return false;

    RemoteMemoryAllocator remoteMemory(remoteProcess);

    if (!remoteMemory.AllocateAndWrite(dllPath.c_str(), dllPath.size()))
      return false;

    RemoteThread remoteThread;
    if (!remoteThread.Create(remoteProcess, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, remoteMemory))
      return false;

    remoteThread.WaitForFinish();
    return remoteThread.GetExitCode() != 0; //return value should be a HMODULE to the loaded module, but a DWORD is not big enough to store one
  }
}