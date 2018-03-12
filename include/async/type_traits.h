#ifndef _ASYNC_TYPE_TRAITS_H
#define _ASYNC_TYPE_TRAITS_H

#include <type_traits>
#include "event.h"

namespace async {

  template<typename T>
  struct is_event_family : public std::false_type
  {};

  template<typename T>
  struct is_event_family<async_declare_event_family<T>> : public std::true_type
  {};

  template<typename T>
  inline constexpr bool is_event_family_v = is_event_family<T>::value;

}
