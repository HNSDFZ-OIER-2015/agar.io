//
// Copyright 2016 riteme
//

#ifndef AGAR_SERVER_NETWORK_HPP_
#define AGAR_SERVER_NETWORK_HPP_

#include "thirdparty/asio.hpp"

#include "Core.hpp"
#include "Processor.hpp"

class AsyncSocket {
 public:
    AsyncSocket(Processor *processor);
    ~AsyncSocket();

    void Open(const tcp::endpoint &endpoint, const size_t maxConnection = 8U);
    void Close();

    void Start();
    void Stop();
    void Boardcast(const DataBuffer &buffer);

 private:
    Processor *_processor = nullptr;
};  // class AsyncSocket

#endif  // AGAR_SERVER_NETWORK_HPP_
