//
// Copyright 2015 riteme
//

#ifndef RILOG_MESSAGE_HPP_
#define RILOG_MESSAGE_HPP_

#include <ctime>
#include <string>
#include <thread>

#include "LogType.hpp"

namespace ri {

struct Message {
    ri::LogType LogType;
    std::time_t RawTime;
    std::string Text;
    std::thread::id ThreadID;
};  // struct Message

}  // namespace ri

#endif  // RILOG_MESSAGE_HPP_
