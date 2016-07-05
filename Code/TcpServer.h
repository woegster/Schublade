#pragma once
#include "SocketEndpoint.h"

namespace toni
{
  class TcpClient;

  class TcpServer
  {
  public:
    bool Start(const SocketEndpoint& localEndpoint, int Backlog);
    TcpClient* Accept();
  private:
    void CleanUp();
  private:
    SOCKET m_listenSocket = INVALID_SOCKET;
    SocketEndpoint m_localEndpoint;
  };
}



