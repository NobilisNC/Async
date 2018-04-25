#ifndef _ASYNC_TASK_H
#define _ASYNC_TASK_H

#include "event_loop.h"

namespace async {

enum class enum_Task {
  Task
};

using FTask = async_declare_event_family(enum_Task);

async_event (FTask, enum_Task::Task ) {
  using function_type = std::function<void()>;
  event(function_type func) : super(), func(func) {}
  function_type func;
};

class simple_task : public async::event_loop<FTask>
{
  public:
    using function_type = std::function<void()>;
    explicit simple_task() = default;
    ~simple_task() override = default;

    inline void task(function_type func) {
      push(new FTask::event<enum_Task::Task>(func));
    }

  protected:
    inline void loop() override {
      std::unique_ptr<FTask::base_event> event(waitEvent());
      if (!event) return;

      if(event->id() == enum_Task::Task)
      {
        FTask::event_type<enum_Task::Task>& action = *FTask::cast<enum_Task::Task>(event);
        action.func();
      }
    }



};

} /* namepace async */

#endif
