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

  int TcpClient::Recv(std::vector<unsigned char>& recvBuffer)
  {
    if (m_connectedSocket != INVALID_SOCKET)
    {
      int retVal = recv(m_connectedSocket, const_cast<char*>(reinterpret_cast<const char*>(recvBuffer.data())), static_cast<int>(recvBuffer.size()), 0);
      if (retVal <= 0)
        Disconnect();
      return retVal;
    }

    return -1;
  }
  
  int TcpClient::Send(const std::vector<unsigned char>& recvBuffer)
  {
    if (m_connectedSocket != INVALID_SOCKET)
    {
      int retVal = send(m_connectedSocket, const_cast<char*>(reinterpret_cast<const char*>(recvBuffer.data())), static_cast<int>(recvBuffer.size()), 0);
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
