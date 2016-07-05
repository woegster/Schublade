#pragma once
#include "networkCommon.h"

namespace toni
{
  class __declspec(dllexport) SocketEndpoint
  {
  public:
    SocketEndpoint();
    void Set(const sockaddr_in& inv4);
    void Set(const ULONG inv4, USHORT port);
    void Set(const sockaddr_in6& inv6);
    void Set(const in6_addr& inv6, USHORT port);
    sockaddr_in6 const * const GetIpV6() const;
    sockaddr_in const * const GetIpV4() const;
  private:
    void ZeroOutSockAddr();
  private:
    int m_addressFamily;
    sockaddr_unified m_SockAddr;
  };

}

