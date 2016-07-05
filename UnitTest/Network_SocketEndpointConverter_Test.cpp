#include "stdafx.h"
#include "CppUnitTest.h"
#include "SocketEndpointConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SchubladenUnitTest
{
  TEST_CLASS(Network_SocketEndpointConverter)
  {
  public:
    TEST_METHOD(FromIPv4)
    {
      m_Testee = toni::SocketEndpointFromIPv4("127.0.0.1", 15);
      Assert::IsNotNull(m_Testee.GetIpV4());
      Assert::IsNull(m_Testee.GetIpV6());
    }

    TEST_METHOD(FromIPv6)
    {
      m_Testee = toni::SocketEndpointFromIPv6("::1", 15);
      Assert::IsNotNull(m_Testee.GetIpV6());
      Assert::IsNull(m_Testee.GetIpV4());
    }

    TEST_METHOD(ToIPv4)
    {
      m_Testee = toni::SocketEndpointFromIPv4("127.0.0.1", 15);
      Assert::AreEqual("127.0.0.1", toni::IPv4FromSocketEndpoint(m_Testee).c_str());
    }

    TEST_METHOD(ToIPv6)
    {
      m_Testee = toni::SocketEndpointFromIPv6("2001:ffff::2", 15);
      Assert::AreEqual("2001:ffff::2", toni::IPv6FromSocketEndpoint(m_Testee).c_str());
    }
  private:
    toni::SocketEndpoint m_Testee;
  };
}