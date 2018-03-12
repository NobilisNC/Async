#include <catch.hpp>

#include <async/sequence.h>
#include <iostream>
namespace TEST {
enum class Command {
    PRINT_1,
    PRINT_2,
    PRINT_3,
    PRINT_4,
    PRINT_5
};

using FCom = async_declare_event_family(Command);

async_event(FCom, Command::PRINT_1)
{
  event(char l) : super(), letter(l) {}

  char letter;
};

async_event(FCom, Command::PRINT_2)
{
  event(char l) : super(), letter(l) {}

  char letter;
};

async_event(FCom, Command::PRINT_3)
{
  event(char l) : super(), letter(l) {}

  char letter;
};

async_event(FCom, Command::PRINT_4)
{
  event(char l) : super(), letter(l) {}

  char letter;
};

async_event(FCom, Command::PRINT_5)
{
  event(char l) : super(), letter(l) {}

  char letter;
};

}

class SL : public async::sequence<TEST::FCom>
{
  public:
    char c[15];

  protected:
    void execute() override {
      using TEST::FCom;
      using TEST::Command ;

      std::unique_ptr<FCom::base_event> event(waitFor(Command::PRINT_1));
      if (!event) return;

      c[0] = FCom::cast<Command::PRINT_1>(event)->letter;

      event.reset( waitFor(Command::PRINT_2) );
      if (!event) return;

      c[1] = FCom::cast<Command::PRINT_2>(event)->letter;

      event.reset( waitFor(Command::PRINT_3) );
      if (!event) return;

      c[2] = FCom::cast<Command::PRINT_3>(event)->letter;

      event.reset( waitFor(Command::PRINT_4) );
      if (!event) return;

      c[3] = FCom::cast<Command::PRINT_4>(event)->letter;

      event.reset( waitFor(Command::PRINT_5) );
      if (!event) return;

      c[4] = FCom::cast<Command::PRINT_5>(event)->letter;
      c[5] = '\0';

    }
};

TEST_CASE("Sequence Loop") {
  using TEST::Command ;
  using TEST::FCom ;
  std::cerr << std::endl
            << "==== TEST CASE [sequence_loop] ===" << std::endl
            << std::endl;
  SL loop;

  loop.start();
  for(int i = 0; i < 4; ++i) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      REQUIRE(loop.isWaitingFor() == Command::PRINT_1);
      std::cerr << "Waiting " << i+1 << " ";
    } std::cerr << std::endl;

  loop.push(new FCom::event<TEST::Command::PRINT_1>('a'));
  loop.push(new FCom::event<Command::PRINT_1>('a'));
  loop.push(new FCom::event<Command::PRINT_1>('a'));

  std::this_thread::sleep_for(std::chrono::seconds(1));

  REQUIRE(loop.isWaitingFor() == Command::PRINT_2);

  loop.push(new FCom::event<Command::PRINT_3>('x'));
  loop.push(new FCom::event<Command::PRINT_2>('b'));
  loop.push(new FCom::event<Command::PRINT_3>('c'));

  loop.push(new FCom::event<Command::PRINT_5>('a'));
  loop.push(new FCom::event<Command::PRINT_4>('d'));

  std::this_thread::sleep_for(std::chrono::seconds(1));

  loop.push(new FCom::event<Command::PRINT_5>('e'));

  std::this_thread::sleep_for(std::chrono::seconds(1));

  REQUIRE(std::string(loop.c) == "abcde");




  loop.stop();
}

