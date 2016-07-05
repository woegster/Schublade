#include "stdafx.h"
#include "SocketEndpoint.h"

namespace toni
{
  SocketEndpoint::SocketEndpoint()
  {
    m_addressFamily = AF_UNSPEC;
    ZeroOutSockAddr();
  }

  void SocketEndpoint::Set(const sockaddr_in& inv4)
  {
    m_addressFamily = AF_INET;
    m_SockAddr.ipv4 = inv4;
  }

  void SocketEndpoint::Set(const ULONG inv4, USHORT port)
  {
    ZeroOutSockAddr();
    m_addressFamily = AF_INET;
    m_SockAddr.ipv4.sin_family = m_addressFamily;
    m_SockAddr.ipv4.sin_addr.s_addr = inv4;
    m_SockAddr.ipv4.sin_port = htons(port);
  }

  void SocketEndpoint::Set(const sockaddr_in6& inv6)
  {
    ZeroOutSockAddr();
    m_addressFamily = AF_INET6;
    m_SockAddr.ipv6 = inv6;
  }

  void SocketEndpoint::Set(const in6_addr& inv6, USHORT port)
  {
    ZeroOutSockAddr();
    m_addressFamily = AF_INET6;
    m_SockAddr.ipv6.sin6_family = m_addressFamily;
    m_SockAddr.ipv6.sin6_addr = inv6;
    m_SockAddr.ipv6.sin6_port = htons(port);
  }

  sockaddr_in6 const * const SocketEndpoint::GetIpV6() const
  {
    if(m_addressFamily == AF_INET6)
      return &m_SockAddr.ipv6;

    return nullptr;
  }

  sockaddr_in const * const SocketEndpoint::GetIpV4() const
  {
    if (m_addressFamily == AF_INET)
      return &m_SockAddr.ipv4;

    return nullptr;
  }

  void SocketEndpoint::ZeroOutSockAddr()
  {
    memset(&m_SockAddr, 0, sizeof(m_SockAddr));
  }
}

