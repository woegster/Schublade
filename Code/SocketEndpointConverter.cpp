#include "stdafx.h"
#include "SocketEndpointConverter.h"

namespace toni
{
  SocketEndpoint SocketEndpointFromIPv4(const std::string& ipv4, USHORT port)
  {
    SocketEndpoint retVal;

    in_addr addr4 = { 0 };
    if (inet_pton(AF_INET, ipv4.c_str(), &addr4) == 1)
    {
      retVal.Set(addr4.s_addr, port);
    }

    return retVal;
  }

  SocketEndpoint SocketEndpointFromIPv6(const std::string& ipv6, USHORT port)
  {
    SocketEndpoint retVal;

    in6_addr addr6 = { 0 };
    if (inet_pton(AF_INET6, ipv6.c_str(), &addr6) == 1)
    {
      retVal.Set(addr6, port);
    }

    return retVal;
  }

  std::string IPv4FromSocketEndpoint(const SocketEndpoint& endpoint)
  {
    std::string retVal;

    sockaddr_in const * const ipv4 = endpoint.GetIpV4();
    if (ipv4)
    {
      char ipv4String[16] = "";
      inet_ntop(AF_INET, const_cast<void*>(reinterpret_cast<const void*>(&ipv4->sin_addr)), ipv4String, sizeof(ipv4String));
      retVal = ipv4String;
    }

    return retVal;
  }

  std::string IPv6FromSocketEndpoint(const SocketEndpoint& endpoint)
  {
    std::string retVal;

    sockaddr_in6 const * const ipv6 = endpoint.GetIpV6();
    if (ipv6)
    {
      char ipv6String[46] = "";
      inet_ntop(AF_INET6, const_cast<void*>(reinterpret_cast<const void*>(&ipv6->sin6_addr)), ipv6String, sizeof(ipv6String));
      retVal = ipv6String;
    }

    return retVal;
  }

  std::string IPv4WithPortFromSocketEndpoint(const SocketEndpoint& endpoint)
  {
    std::string withoutPort = IPv4FromSocketEndpoint(endpoint);    

    sockaddr_in const * const ipv4 = endpoint.GetIpV4();
    if (ipv4)
    {
      std::string withPort = withoutPort;
      withPort += ":";
      withPort += std::to_string(ntohs(ipv4->sin_port));
      return withPort;
    }

    return withoutPort;
  }

  std::string IPv6WithPortFromSocketEndpoint(const SocketEndpoint& endpoint)
  {

    std::string withoutPort = IPv6FromSocketEndpoint(endpoint);
    sockaddr_in6 const * const ipv6 = endpoint.GetIpV6();
    if (ipv6)
    {
      std::string withPort = "[";
      withPort += withoutPort;
      withPort += "]:";
      withPort += std::to_string(ntohs(ipv6->sin6_port));
      return withPort;
    }

    return withoutPort;
  }
}
