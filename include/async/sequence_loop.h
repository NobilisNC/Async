#ifndef _ASYNC_SEQUENCE_LOOP_H
#define _ASYNC_SEQUENCE_LOOP_H

#include "eventloop.h"

namespace async {

template<typename T, typename S>
class sequence_loop : public event_loop<T>
{
  public:
    using status_type = S;
    using enum_type = typename async_get_event_type<T>::enum_type;

    inline status_type status() const { return status_; }
    inline enum_type isWaitingFor() const { return waiting_for_; }

  protected:
    event_type waitFor(enum_type enum_v) {
      event_type event = nullptr;
      waiting_for_ = enum_v;

      do {
        event = waitEvent();
        if(needtoStop()) return nullptr;
        if(!event) continue;
      } while(event->id() != enum_v);

      return event;
    }

    inline void setStatus(status_type status) { status_ = status; }


  private:
    std::atomic<enum_type> waiting_for_;
    std::atomic<status_type> status_;
};




}



#endif
