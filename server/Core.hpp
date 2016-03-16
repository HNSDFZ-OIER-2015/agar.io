//
// Copyright 2016 riteme
//

#ifndef AGAR_SERVER_CORE_HPP_
#define AGAR_SERVER_CORE_HPP_

#include <cstddef>
#include <atomic>
#include <memory>

struct DataBuffer {
    std::shared_ptr<char> pointer;
    size_t size;
};  // struct DataBuffer

struct GameStates {
    auto ToBytes() const -> DataBuffer;
};  // struct GameStates

class AsyncSocket;

class Game {
 public:
    Game();
    ~Game();

    void BindSocket(AsyncSocket *socket);

    void Update();

 protected:
    friend class Processor;

    typedef std::atomic<GameStates *> GameStatesPtr;

    GameStatesPtr _front;
    GameStatesPtr _back;

    AsyncSocket *_socket;
};  // class Game

#endif  // AGAR_SERVER_CORE_HPP_
