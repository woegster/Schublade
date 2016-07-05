#include "stdafx.h"
#include "CppUnitTest.h"
#include "SocketEndpoint.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SchubladenUnitTest
{
  TEST_CLASS(Network_SocketEndpoint)
  {
  public:
    TEST_METHOD(SetV6_byAddr)
    {
      m_Testee.Set(in6addr_any, 24);
      Assert::IsNotNull(m_Testee.GetIpV6());
      Assert::IsNull(m_Testee.GetIpV4());
    }   

    TEST_METHOD(SetV6_byStruct)
    {
      sockaddr_in6 in6Addr = { 0 };
      m_Testee.Set(in6Addr);
      Assert::IsNotNull(m_Testee.GetIpV6());
      Assert::IsNull(m_Testee.GetIpV4());
    }

    TEST_METHOD(SetV4_byAddr)
    {
      m_Testee.Set(INADDR_BROADCAST, 24);
      Assert::IsNull(m_Testee.GetIpV6());
      Assert::IsNotNull(m_Testee.GetIpV4());
    }

    TEST_METHOD(SetV4_byStruct)
    {
      sockaddr_in in4Addr = { 0 };
      m_Testee.Set(in4Addr);
      Assert::IsNull(m_Testee.GetIpV6());
      Assert::IsNotNull(m_Testee.GetIpV4());
    }

    TEST_METHOD(EmptyAfterCtor)
    {
      Assert::IsNull(m_Testee.GetIpV6());
      Assert::IsNull(m_Testee.GetIpV4());
    }
  private:
    toni::SocketEndpoint m_Testee;
  };
}