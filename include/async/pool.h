#ifndef _ASYNC_POOL_H
#define _ASYNC_POOL_H

#include "consummer_thread.h"
#include "queue.h"

namespace async {

template <class tThread>
class pool
{
  public :
    using thread_type = tThread;
    using queue_type = typename thread_type::queue_type;
    using event_type = typename queue_type::value_type;
    using size_type = std::size_t;

    pool(size_type number_of_threads = 0) {
      //add(number_of_threads);
    }
    ~pool() {
      for(auto& thread : threads_) {
        thread->stop();
        delete thread;
      }
    }

    pool(const pool&) = delete;
    pool& operator=(const pool&) = delete;

    /*void add(size_type number = 1) {
      for(size_type i = 0; i < number; ++i) {
        threads_.push_back(new thread_type(queue_));
        threads_.back()->start();
      }
    }*/

    template<typename... Args>
    void add(Args... args) {
      threads_.push_back(new thread_type(queue_, args...));
      threads_.back()->start();
    }

    void remove(size_type number = 1) {
      for(size_type i = 0; i < number; ++i) {
        delete threads_.front();
      }
      threads_.erase(threads_.begin(), threads_.begin() + number);
    }

    inline size_type number() const { return threads_.size(); }

    inline void push(event_type&& event) {queue_.push(event); }
    inline void clear() {
      queue_.clear();
    }
    inline const queue_type& queue() const { return queue_; }

  private:
    queue_type queue_;
    std::vector<thread_type*> threads_;
};



}

#endif //ASYNC_POOL_H
