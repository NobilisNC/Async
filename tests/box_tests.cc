#include <catch.hpp>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <async/consummer_thread.h>

enum class Test { A, B};

using TEST = async_dev::register_family<Test>;

template<>
struct async_dev::event<Test::A> {};

enum class Action {
  SayHello,
  Count
};

using ActionEvent = async_declare_event_family<Action>;

async_declare_event(ActionEvent, Action::SayHello) {
  event(std::string&& name) : base(Action::SayHello), name(name) {}

  std::string name;
};

async_declare_event(ActionEvent, Action::Count) {
  event(int i) : base(Action::Count), i(i) {}

  int i;
};

class ActionBox : public async::consummer_thread<ActionEvent>
{
  public :
    int counter = 0;
    explicit ActionBox(queue_type& queue, std::string name) : consummer_thread(queue), name_(name) {}

  protected :
    void onStart() override {
      //std::cerr << "Hello, I'm Box " << name_ << " and I'm starting" << std::endl;
    }
    void loop() override
    {

      std::unique_ptr<ActionEvent::base> event(waitEvent());
      //std::cerr << "[" << name_ << "] - I received event  : " << (int)event.get() << std::endl;
      if(!event) return;

      if (event->id() == Action::SayHello) {
          auto action = ActionEvent::cast<Action::SayHello>(event.get());
          sayHello(*action);
        } else if(event->id() == Action::Count) {
          auto action = ActionEvent::cast<Action::Count>(event.get());
          count(*action);
        }


    }

    void onEnd() override {
      //std::cerr << "I'm Box " << name_ << " and I'm stopping. Goodbye !" << std::endl;
    }


  private :
    void sayHello(const ActionEvent::event<Action::SayHello>& action) {
      std::cerr << "[" << name_ << "] - Hello " << action.name << std::endl;
    }

    void count(const ActionEvent::event<Action::Count>& action) {
      std::stringstream ss;
      for ( int i = 0; i < action.i; i++) {
          ss << i << ", ";
          counter++;
        }

      //std::cerr << "[" << name_ << "] - Counting until" << ss.str() << std::endl;
    }


  private :
    std::string name_;

};


TEST_CASE("box") {
  std::cerr << std::endl
            << "==== TEST CASE [box] ===" << std::endl
            << std::endl;

  std::srand((unsigned)std::time(nullptr));
  async::queue<ActionEvent> queue;

  queue.push(new ActionEvent::event<Action::Count>(80));

  ActionBox box1(queue, "box1");
  ActionBox box2(queue, "box2");

  box1.start();
  box2.start();

  for (int i = 0 ; i < 5; ++i) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      queue.push(new ActionEvent::event<Action::SayHello>("You shoud see that 5 times {" + std::to_string(i+1) + "}"));
      queue.push(new ActionEvent::event<Action::Count>(10));
    }

  box1.stop();
  box2.stop();

  REQUIRE((box1.counter + box2.counter) == (80 + 10*5) );

}
