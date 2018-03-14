#ifndef _ASYNC_SEQUENCE_LOOP_H
#define _ASYNC_SEQUENCE_LOOP_H

#include "event_loop.h"

namespace async {

template<typename T>
class sequence : public event_loop<T>
{
  public:
    using enum_type  = typename T::enum_type;
    using event_type = typename event_loop<T>::event_type;
    template<enum_type vEnum>
    using e = typename T::template event<vEnum>;

    inline enum_type isWaitingFor() const { return waiting_for_; }

  protected:
    event_type waitFor(enum_type enum_v) {
      event_type event = nullptr;
      waiting_for_ = enum_v;

      do {
        event = event_loop<T>::waitEvent();
        if(thread::needtoStop()) return nullptr;
        if(!event) continue;
      } while(event->id() != enum_v);

      return event;
    }

    template<enum_type vEnum>
    e<vEnum>* waitFor(e<vEnum>* = nullptr) {
        return T::cast<vEnum>(waitFor(vEnum));
    }

    virtual void execute() = 0;

  private:
    void run() override {
      this->onStart();
      execute();
      this->onEnd();
    }

    void loop() override final { /* no used in sequence */ }

  private:
    std::atomic<enum_type> waiting_for_;
};




}



#endif
