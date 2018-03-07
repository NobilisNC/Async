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

    template<T value_t>
    constexpr static auto cast(std::unique_ptr<base>& val) {
      return static_cast<event<value_t>* >(val.get());
    }
};

template<typename T>
struct async_get_event_type { //Default any type
    using type = async::_event<T>;
    using enum_type =  T;
};

template<typename T>
struct async_get_event_type<async::_event<T>> { // event_type
    using type = async::_event<T>;
    using enum_type = decltype(T);
};

template<typename T>
struct async_get_event_type<async_declare_event_family<T>> { // async_declare_event_family
    using type = async::_event<T>;
    using enum_type =  T;
};

template<typename T>
struct async_get_enum_type { // async_declare_event_family
    using type = typename async_get_event_type<T>::type;
};

namespace async_dev {

template<typename T, T t>
struct event {

};

template<typename T, T t>
struct _event {
    _event() : id_(t) {}

    T id_;
};

template<typename T>
struct event_family {
    template<T value>
    using event = _event<T, value>;
};

template<typename T>
using register_family = event_family<T>;

}


#endif
