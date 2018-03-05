#include <catch.hpp>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <async/box.h>

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

class ActionBox : public async::box<ActionEvent>
{
  public :
    explicit ActionBox(queue_type& queue, std::string name) : box(queue), name_(name) {}

  protected :
    void onStart() override {
      std::cerr << "Hello, I'm Box " << name_ << " and I'm starting" << std::endl;
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
      std::cerr << "I'm Box " << name_ << " and I'm stopping. Goodbye !" << std::endl;
    }


    private :
    void sayHello(const ActionEvent::event<Action::SayHello>& action) {
      std::cerr << "[" << name_ << "] - Hello " << action.name << std::endl;
    }

    void count(const ActionEvent::event<Action::Count>& action) {
      std::stringstream ss;
      for ( int i = 0; i < action.i; i++)
          ss << i << ", ";

      std::cerr << "[" << name_ << "] - Counting until" << ss.str() << std::endl;
    }


  private :
    std::string name_;

};


std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

TEST_CASE("box") {
  std::srand(std::time(nullptr));
  async::queue<ActionEvent> queue;

  queue.push(new ActionEvent::event<Action::Count>(80));

  ActionBox box1(queue, "box1");
  ActionBox box2(queue, "box2");

  box1.start();
  box2.start();

  for (int i = 0 ; i < 5; ++i) {
      std::this_thread::sleep_for(std::chrono::seconds(1));

      int nr_event = rand() % 5;
      std::cerr << "[MASTER] -- Generate " << nr_event << " events";

      for (int o = 0; o < nr_event; ++o) {
          if(rand() % 2 == 0) {
              queue.push(new ActionEvent::event<Action::SayHello>(random_string(10)));
            } else {
              queue.push(new ActionEvent::event<Action::Count>(rand() % 1000));
            }
        }


    }

  box1.stop();
  box2.stop();





}
