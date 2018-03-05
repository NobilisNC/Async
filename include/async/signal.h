#ifndef _ASYNC_SIGNAL_H
#define _ASYNC_SIGNAL_H

#include <functional>
#include <vector>
#include <utility>

namespace async {

template<class>
class _signal {};

template<class R, class... Args>
class _signal<R(Args...)>
{
  public:
  using slot_type = std::function<R(Args...)>;
  using vector_type = std::vector<slot_type>;
  using size_type = typename vector_type::size_type;

  _signal(const _signal&) = delete;
  _signal& operator=(const _signal&) = delete;

  explicit _signal() : id_(0) {}
  explicit _signal(_signal&& rhs);

  template<class T>
  void connect(T&& slot) {
    slots_.push_back(std::forward<T>(slot));
  }

  template<class T>
  inline void operator+=(T&& slot) {
    connect(std::forward<T>(slot));
  }


  void operator()(Args const&... args) {
    for(auto& slot : slots_ )
      if(slot) slot(args...);
  }

  private:
  vector_type slots_;
  std::size_t id_;

};

template<typename T>
using signal = _signal<T>;

template<typename Function, typename Object>
constexpr inline auto bind(Function f, Object& o) {
  return std::bind(f, o, std::placeholders::_1);
}


} /* namespace async */


#endif
