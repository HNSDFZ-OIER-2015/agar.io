//
// Copyright 2016 riteme
//

#ifndef AGAR_SERVER_PROCESSOR_HPP_
#define AGAR_SERVER_PROCESSOR_HPP_

#include <cstddef>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <thread>
#include <future>
#include <memory>

#include "Core.hpp"

typedef unsigned command_t;

struct Message {
    size_t size;
    command_t command;
    std::unique_ptr<char> data;
};  // struct Message

class Worker {
 public:
    Worker();
    ~Worker();

    void Start();
    void Async(Message *message);
    void Stop();
    void Join();

    auto IsFree() const -> bool;

 private:
    typedef std::future<Message *> FutureType;

    void _Working(const FutureType &fut);

    std::thread _thread;
    FutureType _future;
};  // class Worker

class Processor {
 public:
    typedef std::unique_ptr<Message> MessagePtr;
    typedef std::function<void(Message *)> CallbackType;

    Processor(Game *game, const size_t workers = 4U);
    ~Processor();

    void Push(const MessagePtr &message);
    void Bind(const command_t command, const CallbackType &callback);

    void Start();
    void Stop();

 private:
    std::vector<Worker> _workers;
    std::queue<MessagePtr> _queue;
    std::unordered_map<command_t, CallbackType> _callbacks;
    Game *_game = nullptr;
};  // class Processor

#endif  // AGAR_SERVER_PROCESSOR_HPP_
