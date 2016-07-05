#pragma once
#include "SocketEndpoint.h"
#include <string>

namespace toni
{
  SocketEndpoint SocketEndpointFromIPv4(const std::string& ipv4, USHORT port);
  SocketEndpoint SocketEndpointFromIPv6(const std::string& ipv6, USHORT port);
  std::string IPv4FromSocketEndpoint(const SocketEndpoint& endpoint);
  std::string IPv6FromSocketEndpoint(const SocketEndpoint& endpoint);
}



