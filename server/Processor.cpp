//
// Copyright 2016 riteme
//

#include "Processor.hpp"

#include <cassert>

#include "thirdparty/logging.hpp"

Worker::Worker() {
    _is_working = false;

    _future = _promise.get_future();
}

Worker::~Worker() = default;

void Worker::Start() {
    _thread = std::thread([this](FutureType &fut) {
        auto hash = std::hash<std::thread::id>();

        LOG.RegisterThread(std::string("Processor Thread #") +
                           std::to_string(hash(std::this_thread::get_id())));

        try {
            this->_Working(fut);
        } catch (const std::exception &e) {
            this->_is_working = false;

            LOG(LOG_ERROR, "A exception occurred: {}", e.what());
            std::rethrow_exception(std::current_exception());
        }

        this->_is_working = false;
    }, _future);
}

void Worker::Stop(bool wait) {
    _promise.set_value({ nullptr, CallbackType() });

    if (wait)
        _thread.join();
    else
        _thread.detach();
}

void Worker::Async(Message *message, const CallbackType &callback) {
    assert(IsFree());
    assert(message != nullptr);

    _promise.set_value({ message, callback });
}

auto Worker::IsFree() const -> bool {
    return !_is_working;
}

void Worker::_Working(FutureType &fut) {
    while (true) {
        auto data = fut.get();

        if (data.first == nullptr)
            return;

        _is_working = true;

        data.second(data.first);

        _is_working = false;
    }
}

Processor::Processor(Game *game, const size_t workers) {
    assert(game != nullptr);

    _game = game;
    _workers.resize(workers);
    _current = _workers.begin();

    _is_running = false;
}

Processor::~Processor() = default;

void Processor::PushMessage(MessagePtr &message) {
    std::lock_guard<std::mutex> guard(_queue_mutex);

    _queue.push(std::move(message));
}

void Processor::BindCallback(const command_t command,
                             const CallbackType &callback) {
    _callbacks[command] = callback;
}

void Processor::Start() {
    for (auto &t : _workers) {
        t.Start();
    }  // foreach in _workers

    _thread = std::thread([this]() {
        auto async = [this](Worker &worker) {
            std::lock_guard<std::mutex> guard(this->_queue_mutex);

            auto current = std::move(this->_queue.front());
            this->_queue.pop();
            worker.Async(current.get(), this->_callbacks[current->command]);
        };

        while (this->_is_running) {
            for (auto &t : this->_workers) {
                if (t.IsFree()) {
                    async(t);
                }
            }  // foreach in this->_workers
        }      // while

        std::this_thread::sleep_for(
            std::chrono::milliseconds(PROCESS_TIME_SPAN));
    });
    _thread.detach();
}

void Processor::Stop() {
    for (auto &t : _workers) {
        t.Stop();
    }  // foreach in _workers

    _is_running = false;
}

auto Processor::IsRunning() const -> bool {
    return _is_running;
}
