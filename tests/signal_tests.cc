#include <catch.hpp>
#include <sstream>
#include<async/signal.h>



constexpr auto OUTPUT = "qwerty";

void output_1(std::ostream& out) {
  out << OUTPUT;
}

std::function<void(std::ostream&)> output_2 = [](std::ostream& out) {
  out << OUTPUT;
};

struct Functor {
    void operator ()(std::ostream& out) const {
      out << OUTPUT;
    }
};

struct O {
    void output(std::ostream& out) {
      out << OUTPUT;
    }
};

using sig = async::signal<void(std::ostream&)>;



TEST_CASE("Signal tests", "[signal]") {

  SECTION("Connecting lambda") {
    sig signal;
    signal.connect([](std::ostream& out){
        out << OUTPUT;
    });

    std::stringstream ss;
    signal(ss);
    REQUIRE(ss.str() == OUTPUT );
  }

  SECTION("Connecting Free Function") {
    sig signal;
    signal.connect(output_1);

    std::stringstream ss;
    signal(ss);
    REQUIRE(ss.str() == OUTPUT );
  }

  SECTION("Connecting std::function") {
    sig signal;
    signal.connect(output_2);

    std::stringstream ss;
    signal(ss);
    REQUIRE(ss.str() == OUTPUT );
  }

  SECTION("Connecting functor") {
    sig signal;
    Functor func;
    signal.connect(func);

    std::stringstream ss;
    signal(ss);
    REQUIRE(ss.str() == OUTPUT );
  }

  SECTION("Connecting member function") {
    sig signal;
    O o;
    signal.connect(async::bind(&O::output, o));

    std::stringstream ss;
    signal(ss);
    REQUIRE(ss.str() == OUTPUT );
  }

}
