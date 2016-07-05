#include "stdafx.h"
#include "CppUnitTest.h"
#include "strOps.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SchubladenUnitTest
{
  TEST_CLASS(strOps_Test_Tokenize)
  {
  public:
    TEST_METHOD(Tokenize_Simple)
    {
      const auto result = toni::Tokenize<std::string>("Apfel,Birne,Clementine,Dattel", ",");
      Assert::AreEqual(4ULL, result.size());
      Assert::AreEqual("Apfel"     , result[0].c_str());
      Assert::AreEqual("Birne"     , result[1].c_str());
      Assert::AreEqual("Clementine", result[2].c_str());
      Assert::AreEqual("Dattel"    , result[3].c_str());
    }
    TEST_METHOD(Tokenize_MultipleTokens)
    {
      const auto result = toni::Tokenize<std::string>("Eins,Zwei;Drei-Vier", ",;-");
      Assert::AreEqual(4ULL, result.size());
      Assert::AreEqual("Eins", result[0].c_str());
      Assert::AreEqual("Zwei", result[1].c_str());
      Assert::AreEqual("Drei", result[2].c_str());
      Assert::AreEqual("Vier", result[3].c_str());
    }
    TEST_METHOD(Tokenize_emptySequence_notRemoved)
    {
      const auto result = toni::Tokenize<std::string>("nix;;dazwischen", ";");
      Assert::AreEqual(3ULL, result.size());
      Assert::AreEqual("nix", result[0].c_str());
      Assert::IsTrue(result[1].empty());
      Assert::AreEqual("dazwischen", result[2].c_str());
    }
    TEST_METHOD(Tokenize_notFound_notSplitted)
    {
      const auto result = toni::Tokenize<std::string>("Hello, World", "#");
      Assert::AreEqual(1ULL, result.size());
      Assert::AreEqual("Hello, World", result[0].c_str());
    }
  };
}