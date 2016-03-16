//
// Copyright 2016 riteme
//

#ifndef AGAR_SERVER_PROCESSOR_HPP_
#define AGAR_SERVER_PROCESSOR_HPP_

#include <cstddef>
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

typedef std::function<void(Message *)> CallbackType;

class Worker {
 public:
    Worker();
    ~Worker();

    void Start();
    void Stop(bool wait = true);
    void Async(Message *message, const CallbackType &callback);

    auto IsFree() const -> bool;

 private:
    typedef std::pair<Message *, CallbackType> TaskType;
    typedef std::promise<TaskType> PromiseType;
    typedef std::future<TaskType> FutureType;

    void _Working(FutureType &fut);

    std::thread _thread;
    PromiseType _promise;
    FutureType _future;
    std::atomic<bool> _is_working;
};  // class Worker

#define PROCESS_TIME_SPAN 16

class Processor {
 public:
    typedef std::unique_ptr<Message> MessagePtr;

    Processor(Game *game, const size_t workers = 4U);
    ~Processor();

    void PushMessage(MessagePtr &message);
    void BindCallback(const command_t command, const CallbackType &callback);

    void Start();
    void Stop();

    auto IsRunning() const -> bool;

 private:
    std::thread _thread;
    std::atomic<bool> _is_running;
    std::vector<Worker> _workers;
    std::vector<Worker>::iterator _current;
    std::mutex _queue_mutex;
    std::queue<MessagePtr> _queue;
    std::unordered_map<command_t, CallbackType> _callbacks;
    Game *_game = nullptr;
};  // class Processor

#endif  // AGAR_SERVER_PROCESSOR_HPP_
