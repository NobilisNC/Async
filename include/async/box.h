#ifndef _ASYNC_BOX_H
#define _ASYNC_BOX_H

#include "queue.h"
#include "thread.h"

namespace async {

template<typename T>
class box : public thread
{
  public :
    using queue_type = async::queue<T>;
    using event_type = typename queue_type::value_type;

    explicit box(queue_type& queue) : queue_(queue) {}
    ~box() = default;

    void stop() override {
      queue_.condition_variable().notify_all();
      thread::stop();
    }


  protected:
    virtual void onStart() {}
    virtual void loop() = 0;
    virtual void onEnd() {}

    event_type waitEvent() {
      {
        std::unique_lock<std::mutex> lk(queue_.mutex());
        queue_.condition_variable().wait(lk , [this](){
            return !queue_.isEmpty() || needtoStop();
          });
      }

      if(!queue_.isEmpty())
        return queue_.pop();

      return nullptr;
    }

    inline queue_type& queue() { return queue_; }

  private:
    void run() override {
      onStart();

      while(!needtoStop())
        loop();

      onEnd();
    }

  private :
    queue_type& queue_;


};

} /* namespace async */


#endif
