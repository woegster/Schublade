#include "CppUnitTest.h"
#include "strOps.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SchubladenUnitTest
{
  TEST_CLASS(strOps_ReplaceAll)
  {
  public:
    TEST_METHOD(ReplaceAll_AllOccurancesReplaced)
    {
      std::string text = "Hans-Peter Hans-Peter Hans-Peter";
      toni::ReplaceAll<std::string>(text, "Peter", "Herbert");
      Assert::AreEqual("Hans-Herbert Hans-Herbert Hans-Herbert", text.c_str());
    }

    TEST_METHOD(ReplaceAll_DirectlyFollowing_Replaced)
    {
      std::string text = "HansHans";
      toni::ReplaceAll<std::string>(text, "Hans", "Peter");
      Assert::AreEqual("PeterPeter", text.c_str());
    }

    TEST_METHOD(ReplaceAll_NotFound_NotModified)
    {
      std::string text = "HansHans";
      toni::ReplaceAll<std::string>(text, "gibts ned", "nix");
      Assert::AreEqual("HansHans", text.c_str());
    }

    TEST_METHOD(ReplaceAll_ReplaceWithNothing)
    {
      std::string text = "Dies/Ist/Ein/Pfad";
      toni::ReplaceAll<std::string>(text, "/", "");
      Assert::AreEqual("DiesIstEinPfad", text.c_str());
    }

    TEST_METHOD(ReplaceAll_ReplaceNothingWithSomething_NoEffect)
    {
      std::string text = "MeinText";
      toni::ReplaceAll<std::string>(text, "", "++");
      Assert::AreEqual("MeinText", text.c_str());
    }
  };
}