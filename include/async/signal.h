#ifndef _ASYNC_SIGNAL_H
#define _ASYNC_SIGNAL_H

#include <functional>
#include <vector>
#include <utility>
#include <algorithm>

namespace async {

template<class>
class _signal {};

template<class Sig>
class connection
{
  private:
    using signal = Sig;
    friend signal;

    signal& signal_;
    size_t id_;

    connection(signal& sig, size_t id)
            : signal_(sig), id_(id)
    {}

  public :

    //connection(connection& other) : signal_(other.signal_), id_(other.id_) {}


    void disconnect() {
      signal_.disconnect(id_);
    }

    void connect() {
      signal_.connect(id_);
    }

    void remove() {
      signal_.remove(id_);
    }

    bool isConnected() {
      return signal_.isConnected(id_);
    }


};

template<class R, class... Args>
class _signal<R(Args...)>
{
    /** Alias for self */
    using class_t = _signal;
    /** Type of slot Id */
    using id_type = std::size_t;


  public:

    /** Slot type std::function<> */
    using slot_type = std::function<R(Args...)>;
    /** Connection type */
    using connection_type = connection< class_t >;
    friend connection_type;

  private:

    /** Struct describing a slot */
    struct handler {
      /** Function to call */
      slot_type slot;

      /** Status of connection */
      bool connected;

      /** Unque id of slot */
      id_type id;
    };

    /** vector_type */
    using vector_type = std::vector<handler>;

  public:

    /* No copy */
    _signal(const _signal&) = delete;
    _signal& operator=(const _signal&) = delete;

    explicit _signal() : id_(0) {}
    explicit _signal(_signal&& rhs) : handlers_(rhs.handlers_), id_(rhs.id_) {}

    /** Connect a slot
         *
         * To be connected the object must be castable to std::function
         * Accepted :
         *   - lambda,
         *   - free function
         *   - std::function
         *   - functor
         *
         * \param slot callable object
         * \return connection object
         */
    template<class T>
    connection_type connect(T&& slot) {
      auto id = gen_id();
      handlers_.push_back({std::forward<T>(slot), true, id});

      return connection_type(*this, id);
    }

    /** Connect a slot
         *
         * Call connect(), but does not return connection
         *
         * \param slot callable object
         */
    template<class T>
    inline void operator+=(T&& slot) {
      connect(std::forward<T>(slot));
    }

    /** Trigger signal
         *
         * Call each slots connected with arguments.
         *
         * \params args arguments to pass for each slot;
         */
    void operator()(Args const&... args) {
      for(auto& handler : handlers_ )
        if(handler.connected && handler.slot) handler.slot(args...);
    }

  private:

    /* Connection API */
    void disconnect(id_type id){
      auto slot = std::find_if(handlers_.begin(), handlers_.end(), [=](const handler& h){
        return id == h.id;
      });

      if (slot == handlers_.end()) return;

      slot->connected = false;
    }

    void connect(id_type id) {
      auto slot = std::find_if(handlers_.begin(), handlers_.end(), [id](const handler& h){
        return id == h.id;
      });

      if (slot == handlers_.end()) return;

      slot->connected = true;
    }

    bool isConnected(id_type id) {
      auto slot = std::find_if(handlers_.begin(), handlers_.end(), [id](const handler& h){
        return id == h.id;
      });

      if (slot == handlers_.end()) return false;

      return slot->connected;
    }

    void remove(id_type id){
      auto slot = std::find_if(handlers_.begin(), handlers_.end(), [id](const handler& h){
        return id == h.id;
      });

      if (slot == handlers_.end()) return;

      handlers_.erase(slot);
    }
    /* generate unique id */
    inline std::size_t gen_id() { return ++id_; }


  private:
    vector_type handlers_;
    id_type id_;
};

template<typename T>
using signal = _signal<T>;


} /* namespace async */


#endif
