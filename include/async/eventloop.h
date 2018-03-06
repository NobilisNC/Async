#ifndef _ASYNC_EVENTLOOP_H
#define _ASYNC_EVENTLOOP_H

#include "box.h"

namespace async {

template<class T>
class event_loop : public box<T>
{
  public:
    using queue_type = typename box::queue_type;
    using event_type = typename box::event_type;
    event_loop() : box(queue_) {}

    inline void push(event_type&& event) {
      queue_.push(std::forward<event_type>(event));
    }

    inline void push(const event_type& event) {
      queue_.push(event);
    }

    inline const queue_type& queue() const { return queue_; }

  private:
    queue_type queue_;
};

}

#endif
