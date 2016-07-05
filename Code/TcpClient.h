#pragma once
#include "SocketEndpoint.h"

namespace toni
{
  class TcpClient
  {
  public:
    TcpClient();
    TcpClient(SOCKET connectedSocket, SocketEndpoint endpointOfSocket);
    ~TcpClient();

    int Recv(void* writeableData, int BufferSize);
    int Send(void const * readableData, int BufferSize);
    const SocketEndpoint& GetEndpoint() const;
    void Disconnect();
  private:
    SOCKET m_connectedSocket = INVALID_SOCKET;
    SocketEndpoint m_remoteAddress;
  };
}