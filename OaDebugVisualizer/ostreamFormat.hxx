// COVINFO FILE: history (access for historical data +esperrer)
#pragma once
#include <sstream>

namespace ostreamFormat_details
{
inline void appendToOstream(std::ostream& ostreamToAppendTo)
{

}

template<typename HeadType, typename... TailType>
inline void appendToOstream(std::ostream& ostreamToAppendTo, HeadType&& head, TailType&& ...tail)
{
  ostreamToAppendTo << head;
  ostreamFormat_details::appendToOstream(ostreamToAppendTo, tail...);
}
}

template<typename HeadType, typename... TailType>
inline std::string ostreamFormat(HeadType&& head, TailType&& ...tail)
{
  std::stringstream output;
  ostreamFormat_details::appendToOstream(output, head, tail...);
  return output.str();
}