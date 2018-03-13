#include <catch.hpp>

#include <async/event.h>


enum class Direction {
  Up,
  Right,
  Down,
  Left
};

using EventDirection = async_declare_event_family(Direction);

async_event(EventDirection, Direction::Up)
{
  event(int d1, int d2) : super(), data1(d1), data2(d2) {}
  ~event() {}

  int data1;
  int data2;
};

async_event(EventDirection, Direction::Right) {
  event(int d) : super(), data(d) {}
  ~event() {}

  int data;
};

async_event(EventDirection, Direction::Down)
{
  event(int d5) : super(), data5(d5) {}
  ~event() {}

  int data5;
};

async_event(EventDirection, Direction::Left)
{
  event(int d8, double d90) : super(), data8(d8), data90(d90) {}
  ~event() {}

  int data8;
  double data90;
};


TEST_CASE("Event test", "[events]") {

  SECTION("Create event") {
    EventDirection::event<Direction::Up> up(1, 2);
    EventDirection::event<Direction::Right> right(50);
    EventDirection::event<Direction::Down> down(10);
    EventDirection::event<Direction::Left> left(0, 2.25);

    REQUIRE((up.data1 == 1  && up.data2 == 2));
    REQUIRE(right.data == 50);
    REQUIRE(down.data5 == 10);
    REQUIRE((left.data8 == 0 && left.data90 == 2.25));

  }

  SECTION("Cast Event") {
    EventDirection::base_event* b = new EventDirection::event<Direction::Down>(10);

    int i = -1;
    if(b->id() == Direction::Down) {
        auto e = EventDirection::cast<Direction::Down>(b);
        i = e->data5;
      }

    REQUIRE(i == 10);
  }

}
