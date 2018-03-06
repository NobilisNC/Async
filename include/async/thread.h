#ifndef _ASYNC_THREAD_H
#define _ASYNC_THREAD_H

#include <memory>
#include <atomic>
#include <thread>

namespace async {

class thread
{
  public:
  thread() : run_(false) {}
  virtual ~thread() {
    run_ = false;
    if (thread_->joinable())
        thread_->join();
  }

  virtual void start() {
    run_ = true;
    thread_.reset(new std::thread(&thread::loop_, this));
  }

  virtual void stop() {
    if(thread_) {
      run_ = false;
      thread_->join();
    }
  }

  protected:
    virtual void run() = 0;
    inline bool needtoStop() const { return !run_; }
    inline void innerStop()  { run_ = false; }

  private:
  inline void loop_() {
    run();
  }

  std::unique_ptr<std::thread> thread_;
  std::atomic<bool> run_;


};

} /* namespace async */

#endif
