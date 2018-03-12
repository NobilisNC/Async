#ifndef _ASYNC_EVENT_H
#define _ASYNC_EVENT_H

#include <type_traits>
#include <memory>

namespace async {

/** @brief Virtual class to handle event
 *
 * @tparam tEnum Enum type
 */
template <class tEnum>
class base_event {
  public :
    // Assertions
    static_assert(std::is_enum_v<tEnum>, "Async event must be specialized with en enum type");

    // Using directive
    using enum_type = tEnum;

    // Ctor & Dtor
    explicit base_event(tEnum value)
            : id_(value) {}
    virtual ~base_event() = 0;


    inline enum_type id() const { return id_; }

  private :
    enum_type id_;
};

template<class tEnum>
base_event<tEnum>::~base_event() {}



#define ASYNC_EVENT_FAMILY_BODY(tEnum) \
template<tEnum> struct event {}; \
\
template<tEnum vEnum> using event_type = event<vEnum>;\
\
template<tEnum vEnum> constexpr static auto cast(base_event* event) {\
    return static_cast<event_type<vEnum>*>(event);\
}\
\
template<tEnum vEnum>\
static constexpr auto cast(std::unique_ptr<base_event>& event) {\
    return cast<vEnum>(event.get());\
}

/** @brief Class to declare a family event
 *
 * This class allows the declaration of an event family
 *
 * @tparam tEnum Enum type
 */
template<class tEnum>
struct event_family
{
    using base_event = async::base_event<tEnum>;
    using enum_type = typename base_event::enum_type;


    ASYNC_EVENT_FAMILY_BODY(tEnum)

  protected:

    template<tEnum vEnum>
    struct super : public base_event {
      constexpr static tEnum enum_value = vEnum;

      super() : base_event(vEnum) {}
      virtual ~super() = 0;
    };
};

template<class tEnum>
template<tEnum vEnum>
event_family<tEnum>::super<vEnum>::~super() {}


#define async_declare_event_family(tEnum) \
struct : public async::event_family<tEnum> { \
    ASYNC_EVENT_FAMILY_BODY(tEnum) \
};

#define async_event(tFamily, vEnum) template<> template<> struct tFamily::event<vEnum> : public tFamily::super<vEnum>

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** ** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** ** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** TYPE TRAITS ** *** *** *** *** *** *** *** *** *** *** *** *** */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** ** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** ** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

template<class, typename = void>
struct is_event_family : public std::false_type
{};

template<class T>
struct is_event_family<T, std::void_t<typename T::enum_type, typename T::base_event>>  : public std::true_type
{};

template<class T>
inline constexpr bool is_event_family_v = is_event_family<T>::value;

} /* namespace async */



#endif
