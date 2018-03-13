#ifndef _ASYNC_THREAD_H
#define _ASYNC_THREAD_H

#include <memory>
#include <atomic>
#include <thread>

namespace async {

class thread
{
  public:
    thread() : need_to_stop_(false) {}
    thread(const thread&) = delete;
    thread& operator =(const thread&) = delete;

    virtual ~thread() {
      need_to_stop_ = true;
      if (thread_->joinable())
        thread_->join();
    }

    virtual void start() {
      need_to_stop_ = false;
      thread_.reset(new std::thread(&thread::loop_, this));
    }

    virtual void notifyStop() {
      need_to_stop_ = true;
    }

    virtual void stop() {
      if(thread_) {
        need_to_stop_ = true;
        thread_->join();
      }
    }

    inline bool isJoinable() const {
      if (!thread_) return false;
      return thread_->joinable();
    }

    inline bool isRunning() const { return is_running_; }

  protected:
    virtual void run() = 0;
    inline bool needtoStop() const { return need_to_stop_; }
    inline void innerStop()  { need_to_stop_ = true; }

  private:
    inline void loop_() {
      is_running_ = true;
      run();
      is_running_ = false;
    }

    std::unique_ptr<std::thread> thread_;
    std::atomic<bool> need_to_stop_;
    std::atomic<bool> is_running_;


};

} /* namespace async */

#endif
