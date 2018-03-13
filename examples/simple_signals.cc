#include <async/signal.h>

#include <iostream>
#include <string>

struct Ui {
  async::signal<void()> onBegin;
  async::signal<void()> onEnd;

  async::signal<void()> onHello;
  async::signal<void(const std::string&)> onError;

  void loop() {
    std::string command;

    // Call signal
    onBegin();

    do {
      std::cin >> command;

      if (command == "hello") {
        onHello();
      } else if(command != "exit" ) {
        onError(command);
      }

    } while (command != "exit");

    onEnd();
  }
};


void displayHelp() {
  std::cout << "You can use \"hello\" or \"exit\"" << std::endl;
}

void explainError(const std::string& str) {
  std::cout << "Error this command does not exists : " << str << std::endl;
}

class People {
  public:
    std::string name;
    void sayHello() {
      std::cout  << "Hello I'm " << name << std::endl;
    }
};

int main() {
  People Max{"Max"};
  People James{"James"};

  Ui ui;

  ui.onBegin.connect(displayHelp);
  ui.onBegin += []() {
    std::cerr << "Hey !" << std::endl;
  };

  ui.onEnd += [](){
    std::cerr << "=== GOODBYE ===" << std::endl;
  };

  ui.onError += explainError;



  ui.onHello.connect([&](){
    Max.sayHello();
  });
  ui.onHello +=[&]() {
    James.sayHello();
  };


  ui.loop();
  return 0;
}
