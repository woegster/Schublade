#include "TcpServer.h"
#include "TcpClient.h"

namespace toni
{
  TcpServer::~TcpServer()
  {
    CleanUp();
  }

  bool TcpServer::Start(const SocketEndpoint& localEndpoint, int Backlog)
  {
    if (m_listenSocket == INVALID_SOCKET)
    {
      m_listenSocket = socket(localEndpoint.GetAddressFamily(), SOCK_STREAM, IPPROTO_TCP);
      if (m_listenSocket != INVALID_SOCKET)
      {
        if (bind(m_listenSocket, localEndpoint.GetIpGeneric(), static_cast<int>(localEndpoint.GetGenericIpSize())) == 0)
        {
          if (listen(m_listenSocket, Backlog) == 0)
          {
            m_localEndpoint = localEndpoint;
            return true;
          }            
        }

        CleanUp();
      }
    }

    return false;
  }

  TcpClient* TcpServer::Accept()
  {
    if (m_listenSocket != INVALID_SOCKET)
    {
      char sockAddrData[sizeof(sockaddr_in6)] = { 0 };
      int sockAddrData_Size = sizeof(sockAddrData);
      SOCKET remoteSocket = accept(m_listenSocket, reinterpret_cast<sockaddr*>(sockAddrData), &sockAddrData_Size);
      
      if (remoteSocket != INVALID_SOCKET)
      {
        SocketEndpoint clientEndpoint;
        switch (m_localEndpoint.GetAddressFamily())
        {
        case AF_INET:
          clientEndpoint.Set(*reinterpret_cast<sockaddr_in*>(sockAddrData));
          break;
        case AF_INET6:
          clientEndpoint.Set(*reinterpret_cast<sockaddr_in6*>(sockAddrData));
          break;
        }

        return new TcpClient(remoteSocket, clientEndpoint);
      }
    }

    return nullptr;
  }

  void TcpServer::CleanUp()
  {
    if (m_listenSocket != INVALID_SOCKET)
    {
      closesocket(m_listenSocket);
      m_listenSocket = INVALID_SOCKET;
    }
  }
}