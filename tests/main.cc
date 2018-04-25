#define CATCH_CONFIG_MAIN
#include <catch.hpp>



#include <async/task.h>




TEST_CASE("plop2", "[plop]") {

  async::simple_task T;
  T.start();



  T.task([](){
      std::cerr << "==========================================================" << "Hello World !" << std::endl;
    });

  std::this_thread::sleep_for(std::chrono::seconds(5));
}
