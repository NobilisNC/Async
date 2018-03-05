#ifndef _ASYNC_QUEUE_H
#define _ASYNC_QUEUE_H

#include <mutex>
#include <queue>
#include "event.h"
#include "signal.h"

namespace async {

  template<typename value_t>
  class Queue
  {
    using queue_t =  std::queue<value_t>;

    public :
      using value_type = value_t;

      Queue() = default;
      ~Queue() = default;

      inline bool isEmpty() const {
          std::lock_guard<std::mutex> lguard(mutex_);
          return queue_.empty();
      }

      inline typename queue_t::size_type size() const {
          std::lock_guard<std::mutex> lguard(mutex_);
          return queue_.size();
      }

      void push(const value_t& value) {
        std::lock_guard<std::mutex> lguard(mutex_);
        queue_.push(value);
        onNewElement();
      }

      void push(value_t&& value) {
        std::lock_guard<std::mutex> lguard(mutex_);
        queue_.push(std::forward<value_t>(value));
        onNewElement();
      }

      value_t pop() {
        std::lock_guard<std::mutex> lguard(mutex_);
        value_t val = queue_.front();
        queue_.pop();
        return val;
      }

    signal<void()> onNewElement;
    private:
      queue_t queue_;
      mutable std::mutex mutex_;

  };

  template<typename T>
  class Queue<async_declare_event_family<T>> : public Queue<_event<T>*> {};

  template<typename T>
  class Queue<_event<T>> : public Queue<_event<T>*> {};

} /* namespace async */

#endif
