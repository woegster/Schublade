#pragma once

template<class... Types>
class TypeList
{
public:
  template<class Callable>
  constexpr static inline void for_each(Callable&& callback) noexcept
  {
    consumer<Callable, Types...>(callback);
  }
private:
  template<class Callable, class Head, class... Tail>
  constexpr static inline void consumer(Callable& callback) noexcept
  {
    Head* dummy = nullptr;
    callback(dummy);
    if constexpr (sizeof...(Tail) > 0)
    {
      consumer<Callable, Tail...>(callback);
    }    
  }
};

// Used to iterate all types in the the TypeList
// TypeList<int, float, std::string>::for_each([](auto* dummy)
// {
//   using Type = std::remove_pointer_t<decltype(dummy)>;
//   //Do something with Type
// });
