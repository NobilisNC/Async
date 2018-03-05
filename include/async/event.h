#ifndef _ASYNC_EVENT_H
#define _ASYNC_EVENT_H

#include <type_traits>

namespace async {

template<typename Id>
class _event
{
  static_assert(std::is_enum<Id>::value, "ID type must be an enum type");

  public :
    _event(Id id) : id_(id) {}
    virtual ~_event() = 0;

    inline const Id& id() const { return id_; }

  private :
    Id id_;
};

template<typename Id>
_event<Id>::~_event() {}


#define async_declare_event(family, enum_v) template<> template<> \
  struct family ## ::event< enum_v > : public family ## :: base


} /* namespace async */

template<typename T>
struct async_declare_event_family {
    /* Type of enum */
    using enum_t = T;
    static_assert(std::is_enum<enum_t>::value, "ID type for Async::Event must be an enum type");

    /* base event */
    using base = async::_event<T>;
    /* base event ptr */
    using base_ptr = async::_event<T>*;

    /* template to specialize */
    template<T>
    struct event {};

    /* cast */
    template<T value_t>
    constexpr static auto cast(base_ptr val) {
      return static_cast<event<value_t>* >(val);
    }

};

#endif
