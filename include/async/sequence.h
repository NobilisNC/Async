#ifndef _ASYNC_SEQUENCE_LOOP_H
#define _ASYNC_SEQUENCE_LOOP_H

#include "eventloop.h"

namespace async {

template<typename T>
class sequence : public event_loop<T>
{
  public:
    using enum_type = typename async_get_event_type<T>::enum_type;

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

    virtual void execute() = 0;

  private:
    void run() override {
      onStart();
      execute();
      onEnd();
    }

    void loop() override final { /* no used in sequence */ }

  private:
    std::atomic<enum_type> waiting_for_;
};




}



#endif
