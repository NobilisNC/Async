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
    signal.connect([&](std::ostream& x){
        o.output(x);
      });

    std::stringstream ss;
    signal(ss);
    REQUIRE(ss.str() == OUTPUT );
  }

}


TEST_CASE("Connection", "[signal]") {
  SECTION("One connection") {
    async::signal<void(int&)> signal;

    int i = 0;
    auto connection = signal.connect([](int& i) {i++;});

    signal(i);

    REQUIRE(i == 1);

    connection.disconnect();
    signal(i);

    REQUIRE(i == 1);

    connection.connect();
    signal(i);

    REQUIRE(i == 2);

    connection.remove();
    signal(i);

    REQUIRE(i == 2);

    connection.connect();
    signal(i);

    REQUIRE(i == 2);
  }

  SECTION("Multi connection") {
    async::signal<void(int&)> signal;

    auto c1 = signal.connect([](int& i) {
        i += 1;
      });

    auto c2 =  signal.connect([](int& i) {
        i += 2;
      });

    auto c3 = signal.connect([](int& i) {
           i += 3;
      });

    int i = 0;
    signal(i);

    REQUIRE(i == 6);

    c1.disconnect();
    REQUIRE(!c1.isConnected());
    i = 0;
    signal(i);
    REQUIRE( i == 5 );

    c1.connect();
    c2.remove();

    i = 0;
    signal(i);

    REQUIRE(i == 4);
  }
}
