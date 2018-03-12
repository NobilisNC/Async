#ifndef _ASYNC_QUEUE_H
#define _ASYNC_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>
#include "event.h"
#include "signal.h"

namespace async {

template<typename value_t>
class _queue
{
    using queue_t = std::queue<value_t>;

  public :
    using value_type = value_t;

    _queue() = default;
    ~_queue() = default;

    inline bool isEmpty() const {
      std::lock_guard<std::mutex> lguard(queue_mutex_);
      return queue_.empty();
    }

    inline typename queue_t::size_type size() const {
      std::lock_guard<std::mutex> lguard(queue_mutex_);
      return queue_.size();
    }

    void push(const value_t& value) {
      {
        std::lock_guard<std::mutex> lguard(queue_mutex_);
        queue_.push(value);
      }
      {
        std::lock_guard<std::mutex> lguardcv(cv_mutex_);
        cv_.notify_one();
      }
    }

    void push(value_t&& value) {
      {
        std::lock_guard<std::mutex> lguard(queue_mutex_);
        queue_.push(std::forward<value_t>(value));
      }
      {
        std::lock_guard<std::mutex> lguardcv(cv_mutex_);
        cv_.notify_one();
      }
    }

    inline void clear() {
        std::lock_guard<std::mutex> lguard(queue_mutex_);
        queue_ = {};
    }

    value_t pop() {
      std::lock_guard<std::mutex> lguard(queue_mutex_);
      value_t val = queue_.front();
      queue_.pop();
      return val;
    }

    inline std::condition_variable& condition_variable() {
      return cv_;
    }

    inline std::mutex& mutex() {
      return cv_mutex_;
    }

  private:
    queue_t queue_;
    mutable std::mutex queue_mutex_;
    mutable std::mutex cv_mutex_;
    std::condition_variable cv_;
};

template<typename T>
using queue = _queue<std::conditional_t<is_event_family_v<T>, typename T::base_event*, T>>;

} /* namespace async */

#endif
