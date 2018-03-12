#ifndef _ASYNC_EVENTLOOP_H
#define _ASYNC_EVENTLOOP_H

#include "consummer_thread.h"

namespace async {

template<class T>
class event_loop : public consummer_thread<T>
{
  public:
    using queue_type = typename consummer_thread<T>::queue_type;
    using event_type = typename consummer_thread<T>::event_type;

    event_loop() : consummer_thread<T>(queue_) {}

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
