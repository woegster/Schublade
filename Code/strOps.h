#pragma once
#include <string>
#include <vector>

namespace toni
{
  template<class stringClass>
  void ReplaceAll(stringClass& source, const stringClass& what, const stringClass& with)
  {
    auto foundPos = source.find(what);
    while (foundPos != stringClass::npos)
    {
      source.replace(foundPos, what.length(), with);
      foundPos = source.find(what, foundPos);
    }
  }

  template<class stringClass>
  std::vector<stringClass> Tokenize(const stringClass& source, const stringClass& tokens)
  {
    std::vector<stringClass> result;

    size_t prevTok = 0;
    while (prevTok != stringClass::npos)
    {
      auto nextTok = source.find_first_of(tokens, prevTok);

      result.push_back(source.substr(prevTok, nextTok - prevTok));

      if (nextTok == stringClass::npos)
        break;

      prevTok = nextTok + 1;
    }

    return result;
  }
}
