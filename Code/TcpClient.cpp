#include "stdafx.h"
#include "TcpClient.h"

namespace toni
{
  TcpClient::TcpClient()
  {

  }

  TcpClient::TcpClient(SOCKET connectedSocket, SocketEndpoint endpointOfSocket)
    : m_connectedSocket(connectedSocket)
    , m_remoteAddress(endpointOfSocket)
  {

  }

  TcpClient::~TcpClient()
  {
    Disconnect();
  }

  int TcpClient::Recv(void* writeableData, int BufferSize)
  {
    if (m_connectedSocket != INVALID_SOCKET)
    {
      int retVal = recv(m_connectedSocket, static_cast<char*>(writeableData), BufferSize, 0);
      if (retVal <= 0)
        Disconnect();
      return retVal;
    }

    return -1;
  }
  
  int TcpClient::Send(void const * readableData, int BufferSize)
  {
    if (m_connectedSocket != INVALID_SOCKET)
    {
      int retVal = send(m_connectedSocket, static_cast<const char*>(readableData), BufferSize, 0);
      if (retVal <= 0)
        Disconnect();
      return retVal;
    }

    return -1;
  }

  void TcpClient::Disconnect()
  {
    if (m_connectedSocket != INVALID_SOCKET)
    {
      closesocket(m_connectedSocket);
      m_connectedSocket = INVALID_SOCKET;
    }
  }

  const SocketEndpoint& TcpClient::GetEndpoint() const
  {
    return m_remoteAddress;
  }
}
