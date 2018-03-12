#include <async/event.h>
#include <iostream>


namespace abc {
enum class enum1 {
    a, b, c
};


using Fenum1 = async_declare_event_family(enum1)

async_event(Fenum1, enum1::a) {
  char c;
  event(char c) : super(), c(c) {}
};

async_event(Fenum1, enum1::b) {
  char c;
  event(char c) : super(), c(c) {}
};

async_event(Fenum1, enum1::c) {
  char c;
  event(char c) : super(), c(c) {}
};


}


int main() {

  abc::Fenum1::base_event* event1 = new abc::Fenum1::event<abc::enum1::a>('a');
  abc::Fenum1::base_event* event2 = new abc::Fenum1::event<abc::enum1::b>('b');
  abc::Fenum1::base_event* event3 = new abc::Fenum1::event<abc::enum1::c>('c');

  std::cerr << (int)event1->id() << " " << (int)event2->id() << " " << (int)event3->id() << std::endl;

  abc::Fenum1::event<abc::enum1 ::b>* _b = abc::Fenum1::cast<abc::enum1::b>(event2);
  std::cerr << _b->c << std::endl;


  return 0;
}


