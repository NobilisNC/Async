#include <catch.hpp>
#include <iostream>
#include <chrono>

#include <async/event_loop.h>

enum class Action {
    SayHello,
    Task
};

using ActionF = async_declare_event_family(Action);

async_event(ActionF, Action::SayHello)
{
  event(std::string name) : super(), name_(name) {}

  std::string name_;
};

async_event(ActionF, Action::Task)
{
  event(std::function<void()> task) : super(), task_(task) {}

  std::function<void()> task_;
};


class Loop : public async::event_loop<ActionF>
{
  public :
    Loop() = default;
    ~Loop() = default;

    void onStart() override {
      std::cerr << "Loop started" << std::endl;
    }

    void loop() override {
      //std::cerr << "=== Loop ==== " << std::this_thread::get_id() << std::endl;
      std::unique_ptr<ActionF::base_event> event(waitEvent());

      if (!event) return;


      if(event->id() == Action::SayHello) {
        std::cerr << "Hello " << ActionF::cast<Action::SayHello>(event.get())->name_ << std::endl;
      } else if ( event->id() == Action::Task) {
        ActionF::cast<Action::Task>(event.get())->task_();
      }
    }
};




TEST_CASE("test event_loop") {
  std::cerr << std::endl
            << "==== TEST CASE [event_loop] ===" << std::endl
            << std::endl;

  Loop l;
  l.start();

  l.push(new ActionF::event<Action::SayHello>("Gerard"));
  l.push(new ActionF::event<Action::SayHello>("Bastien"));
  l.push(new ActionF::event<Action::SayHello>("Ludivine"));
  l.push(new ActionF::event<Action::SayHello>("Tom"));

  l.push(new ActionF::event<Action::Task>([&]() {
    std::cerr << "Task :)" << std::endl;
    std::cerr << "I will say hello" << std::endl;
    l.push(new ActionF::event<Action::SayHello>("TASK"));

    std::cerr << "I will sleep for 5 seconds" << std::endl;
    for (int i = 0; i < 5; i++) {
      std::cerr << i << ' ';
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cerr << "Im wake up ;)" << std::endl;
  }));

  l.push(new ActionF::event<Action::SayHello>("you will never see me"));
  l.push(new ActionF::event<Action::SayHello>("Tom3"));
  l.push(new ActionF::event<Action::SayHello>("Tom4"));
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  l.clear();
  while(!l.queue().isEmpty()) {
    //std::cerr << "Loop Queue : " << l.queue().size() << "   " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  std::cerr << "Loop is empty" << std::endl;
  l.stop();
}
