#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <ctime>
#include "ostreamFormat.hxx"
#include "ConfigMapper.hxx"

#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")

#define ADDIN_API __declspec(dllexport)

typedef struct tagDEBUGHELPER
{
  DWORD dwVersion;
  HRESULT(WINAPI *ReadDebuggeeMemory)(struct tagDEBUGHELPER *pThis, DWORD dwAddr, DWORD nWant, VOID* pWhere, DWORD *nGot);
  // from here only when dwVersion >= 0x20000
  DWORDLONG(WINAPI *GetRealAddress)(struct tagDEBUGHELPER *pThis);
  HRESULT(WINAPI *ReadDebuggeeMemoryEx)(struct tagDEBUGHELPER *pThis, DWORDLONG qwAddr, DWORD nWant, VOID* pWhere, DWORD *nGot);
  int (WINAPI *GetProcessorType)(struct tagDEBUGHELPER *pThis);
} DEBUGHELPER;

typedef HRESULT(WINAPI *CUSTOMVIEWER)(DWORD dwAddress, DEBUGHELPER *pHelper, int nBase, BOOL bUniStrings, char *pResult, size_t max, DWORD reserved);

struct PvssTime
{
  __time64_t time;
  std::int32_t nanos;
  std::uint16_t count;
};

struct DpIdentifier
{
  char vtable[8];
  std::uint32_t sysnum;
  std::uint32_t dp;
  std::int16_t el;
  std::int32_t configNr;
  std::int16_t detail;
  std::int32_t attr;
  std::int16_t type;
};

struct ProtobufTime
{
  char vtable[8];
  char metadataptr[8];
  std::int64_t seconds;
  std::int32_t nanoseconds;
};

#define MAGIC_MEMORY_READ(target) {\
  auto realAddress = pHelper->GetRealAddress(pHelper);\
  if(realAddress != 0)\
  {\
    DWORD bytesReadFromMemory = 0;\
    if(FAILED(pHelper->ReadDebuggeeMemoryEx(pHelper, realAddress, sizeof(target), &target, &bytesReadFromMemory)) || bytesReadFromMemory != sizeof(target))\
    {\
      return E_FAIL;\
    }\
  }\
  else\
    return E_FAIL;\
}


extern "C" ADDIN_API HRESULT PvssTimeFormatter(DWORD dwAddress, DEBUGHELPER *pHelper, int nBase, BOOL bUniStrings, char *pResult, size_t max, DWORD reserved)
{
  PvssTime timeToVisualize;
  MAGIC_MEMORY_READ(timeToVisualize);
  
  char formattedTime[128] = "";
  strftime(formattedTime, sizeof(formattedTime), "%d.%m.%Y %H:%M:%S", gmtime(&timeToVisualize.time));

  std::string fullFormat = ostreamFormat(formattedTime, " UTC +", timeToVisualize.nanos, "ns #", timeToVisualize.count);

  sprintf_s(pResult, max, "%s", fullFormat.c_str());
  return S_OK;
}

extern "C" ADDIN_API HRESULT DpIdentifierFormatter(DWORD dwAddress, DEBUGHELPER *pHelper, int nBase, BOOL bUniStrings, char *pResult, size_t max, DWORD reserved)
{
  DpIdentifier id;
  MAGIC_MEMORY_READ(id);

  WSADATA wdat;
  WSAStartup(MAKEWORD(2, 2), &wdat);

  SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  
  SOCKADDR_IN debugHelperManager;
  memset(&debugHelperManager, 0, sizeof(debugHelperManager));
  debugHelperManager.sin_family = AF_INET;
  debugHelperManager.sin_addr.s_addr = inet_addr("127.0.0.1");
  debugHelperManager.sin_port = htons(50299);

  sendto(s, (char*)&id, sizeof(id), 0, (sockaddr*)&debugHelperManager, sizeof(debugHelperManager));

  char translatedName[512] = "";
  sockaddr_in recvdFrom;
  int formLen = sizeof(recvdFrom);
  recvfrom(s, (char*)&translatedName, sizeof(translatedName), 0, (sockaddr*)&recvdFrom, &formLen);

  if (translatedName[0] != 0)
  {
    sprintf_s(pResult, max, "%s", translatedName);
    return S_OK;
  }
  else
  {
    if (id.configNr > 0)
    {
      auto cfgName = getConfigName(id.configNr);
      auto attrName = getAttributeName(id.configNr, id.attr);
      if (id.detail > 0)
      {
        sprintf_s(pResult, max, "%u:%u.%d.%s.%d.%s", id.sysnum, id.dp, id.el, cfgName.c_str(), id.detail, attrName.c_str());
      }
      else
      {
        sprintf_s(pResult, max, "%u:%u.%d.%s..%s", id.sysnum, id.dp, id.el, cfgName.c_str(), attrName.c_str());
      }
    }
    else
    {
      sprintf_s(pResult, max, "%u:%u.%d", id.sysnum, id.dp, id.el);
    }
  }

  WSACleanup();  
  return S_OK;
}

extern "C" ADDIN_API HRESULT ProtobufTimeFormatter(DWORD dwAddress, DEBUGHELPER *pHelper, int nBase, BOOL bUniStrings, char *pResult, size_t max, DWORD reserved)
{
  ProtobufTime timeToVisualize;
  MAGIC_MEMORY_READ(timeToVisualize);

  char formattedTime[128] = "";
  strftime(formattedTime, sizeof(formattedTime), "%d.%m.%Y %H:%M:%S", gmtime(&timeToVisualize.seconds));

  std::string fullFormat = ostreamFormat(formattedTime, " UTC +", timeToVisualize.nanoseconds, "ns");

  sprintf_s(pResult, max, "%s", fullFormat.c_str());
  return S_OK;
}