#include <catch.hpp>
#include <async/pool.h>
#include <cstring>
#include <iostream>



void SieveOfEratosthenes(int n, std::vector<int>& primes)
{
  // Create a boolean array "prime[0..n]" and initialize
  // all entries it as true. A value in prime[i] will
  // finally be false if i is Not a prime, else true.
  bool prime[n+1];
  memset(prime, true, sizeof(prime));

  for (int p=2; p*p<=n; p++)
  {
    // If prime[p] is not changed, then it is a prime
    if (prime[p])
    {
      // Update all multiples of p
      for (int i=p*2; i<=n; i += p)
        prime[i] = false;
    }
  }

  // Print all prime numbers
  for (int p=2; p<=n; p++)
    if (prime[p])
      primes.push_back(p);
}


enum class Action {
    Prime
};

using FAction = async_declare_event_family(Action)

async_event(FAction, Action::Prime)
{
  int limit;
  event(int limit) : super(), limit(limit) {}
};


class MyThread : public async::consummer_thread<FAction>
{
  public :
    using queue_type = typename async::consummer_thread<FAction>::queue_type;
    MyThread(queue_type& queue, std::string name) : consummer_thread(queue), name_(name) {
      std::cerr << "Creation of : " << name_ << std::endl;
    }
    ~MyThread() {
      std::cerr << "Destruction of : " << name_ << std::endl;
    }



  private:
    void onStart() final {
      std::cerr <<  name_ << " is going to work ! " << std::endl;
    }
    void loop() final {
      std::unique_ptr<FAction::base_event> event(waitEvent());
      if (!event) return;

      if(event->id() == Action::Prime)
      {
        FAction::event_type<Action::Prime>& action = *FAction::cast<Action::Prime>(event);
        std::vector<int> primes;
        SieveOfEratosthenes(action.limit, primes);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cerr << name_ << " with order " << action.limit << " found " << primes.size() << " primes numer" << std::endl;
      }


    }

    void onEnd() final {
      std::cerr <<  name_ << " is going to sleep ! Zzz " << std::endl;
    }


    std::string name_;
};



TEST_CASE("pool test", "[pool]") {
  std::cerr << std::endl
            << "==== TEST CASE [pool] ===" << std::endl
            << std::endl;

  async::pool<MyThread> pool;
  pool.add("Thread1");
  pool.add("Thread2");
  pool.add("Thread3");
  pool.add("Thread4");
  pool.add("Thread5");

  pool.push(new FAction::event<Action::Prime>(10000));
  pool.push(new FAction::event<Action::Prime>(10000));
  pool.push(new FAction::event<Action::Prime>(100000));
  pool.push(new FAction::event<Action::Prime>(100000));
  pool.push(new FAction::event<Action::Prime>(100));
  pool.push(new FAction::event<Action::Prime>(100));
  pool.push(new FAction::event<Action::Prime>(100000));


  while(!pool.queue().isEmpty()) {}
}
