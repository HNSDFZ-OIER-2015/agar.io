//
// Copyright 2016 riteme
//

#ifndef AGAR_SERVER_CORE_HPP_
#define AGAR_SERVER_CORE_HPP_

#include <cstddef>
#include <atomic>
#include <memory>

#include "Network.hpp"

struct DataBuffer {
    std::shared_ptr<char> pointer;
    size_t size;
};  // struct DataBuffer

struct GameStates {
    auto ToBytes() const -> DataBuffer;
};  // struct GameStates

class Game {
 public:
    Game();
    ~Game();

    void Update();
    void Sendout(const AsyncSocket &socket);

 protected:
    friend class Processor;

    typedef std::atomic<GameStates *> GameStatesPtr;

    GameStatesPtr _front;
    GameStatesPtr _back;
};  // class Game

#endif  // AGAR_SERVER_CORE_HPP_
