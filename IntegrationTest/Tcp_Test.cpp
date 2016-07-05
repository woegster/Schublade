#include "stdafx.h"
#include "CppUnitTest.h"
#include "TcpServer.h"
#include "TcpClient.h"
#include "SocketEndpointConverter.h"
#include <thread>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTest
{
  TEST_CLASS(TcpTest)
  {
  public:
    TEST_CLASS_INITIALIZE(TcpTest_Init)
    {
      WSADATA wsaData = { 0 };
      Assert::AreEqual(0, WSAStartup(MAKEWORD(2, 2), &wsaData));
    }

    TEST_METHOD(TcpTest_AcceptWithNoInit_Fails)
    {
      toni::TcpServer tcpSrv;
      Assert::IsNull(tcpSrv.Accept());
    }

    TEST_METHOD(TcpTest_IoOnDisconnectedClient_Fails)
    {
      toni::TcpClient clnt;
      std::vector<char> bfr(128);
      Assert::AreEqual(-1, clnt.Recv(&bfr[0], (int)bfr.size()));
      Assert::AreEqual(-1, clnt.Send(&bfr[0], (int)bfr.size()));
    }

    TEST_METHOD(TcpTest_StartV4)
    {
      toni::TcpServer tcpSrv;
      Assert::IsTrue(tcpSrv.Start(toni::SocketEndpointFromIPv4("127.0.0.1", m_unitTestPort), 1));
    }

    TEST_METHOD(TcpTest_StartV6)
    {
      toni::TcpServer tcpSrv;
      Assert::IsTrue(tcpSrv.Start(toni::SocketEndpointFromIPv6("::1", m_unitTestPort), 1));
    }

    TEST_METHOD(TcpTest_StartAndConnectV4)
    {
      TestServerClient_Connect(toni::SocketEndpointFromIPv4("127.0.0.1", m_unitTestPort));
    }

    TEST_METHOD(TcpTest_StartAndConnectV6)
    {
      TestServerClient_Connect(toni::SocketEndpointFromIPv6("::1", m_unitTestPort));
    }

    void TestServerClient_Connect(const toni::SocketEndpoint& endPoint)
    {
      toni::TcpServer tcpServer;
      Assert::IsTrue(tcpServer.Start(endPoint, 5));

      std::thread clntThread([&]()
      {
        toni::TcpClient client;
        Assert::IsTrue(client.ConnectTo(endPoint));
      });

      toni::TcpClient* clntInThread = tcpServer.Accept();
      Assert::IsNotNull(clntInThread);
      clntThread.join();
      delete clntInThread;
    }

    TEST_METHOD(TcpTest_SendToClientV4)
    {
      TestServerClient_SendToClient(toni::SocketEndpointFromIPv4("127.0.0.1", m_unitTestPort));
    }

    TEST_METHOD(TcpTest_SendToClientV6)
    {
      TestServerClient_SendToClient(toni::SocketEndpointFromIPv6("::1", m_unitTestPort));
    }   

    void TestServerClient_SendToClient(const toni::SocketEndpoint& endPoint)
    {
      toni::TcpServer tcpServer;
      Assert::IsTrue(tcpServer.Start(endPoint, 5));

      std::thread clntThread([&]()
      {
        toni::TcpClient client;
        Assert::IsTrue(client.ConnectTo(endPoint));

        std::vector<char> recvBuffer(128);
        client.Recv(&recvBuffer[0], (int)recvBuffer.size());

        Assert::AreEqual("ServerToClient", &recvBuffer[0]);
      });

      toni::TcpClient* clntInThread = tcpServer.Accept();
      Assert::IsNotNull(clntInThread);

      std::string serverToClientMsg = "ServerToClient";
      clntInThread->Send(serverToClientMsg.data(), (int)serverToClientMsg.length());

      clntThread.join();
      delete clntInThread;
    }

    TEST_METHOD(TcpTest_SendToServerV4)
    {
      TestServerClient_SendToServer(toni::SocketEndpointFromIPv4("127.0.0.1", m_unitTestPort));
    }

    TEST_METHOD(TcpTest_SendToServerV6)
    {
      TestServerClient_SendToServer(toni::SocketEndpointFromIPv6("::1", m_unitTestPort));
    }

    void TestServerClient_SendToServer(const toni::SocketEndpoint& endPoint)
    {
      toni::TcpServer tcpServer;
      Assert::IsTrue(tcpServer.Start(endPoint, 5));

      std::thread clntThread([&]()
      {
        toni::TcpClient client;
        Assert::IsTrue(client.ConnectTo(endPoint));

        std::string clientToServer = "ClientToServer";
        client.Send(clientToServer.data(), (int)clientToServer.length());
      });

      toni::TcpClient* clntInThread = tcpServer.Accept();
      Assert::IsNotNull(clntInThread);

      std::vector<char> recvBuffer(128);
      clntInThread->Recv(&recvBuffer[0], (int)recvBuffer.size());
      Assert::AreEqual("ClientToServer", &recvBuffer[0]);

      clntThread.join();
      delete clntInThread;
    }

    TEST_CLASS_CLEANUP(TcpTest_Cleanup)
    {
      WSACleanup();
    }
  private:
    int m_unitTestPort = 12345;
  };
}