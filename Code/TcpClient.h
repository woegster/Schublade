#pragma once
#include "SocketEndpoint.h"
#include <vector>

namespace toni
{
  class TcpClient
  {
  public:
    TcpClient();
    TcpClient(SOCKET connectedSocket, SocketEndpoint endpointOfSocket);
    ~TcpClient();

    int Recv(std::vector<unsigned char>& recvBuffer);
    int Send(const std::vector<unsigned char>& recvBuffer);
    const SocketEndpoint& GetEndpoint() const;
    void Disconnect();
  private:
    SOCKET m_connectedSocket = INVALID_SOCKET;
    SocketEndpoint m_remoteAddress;
  };
}