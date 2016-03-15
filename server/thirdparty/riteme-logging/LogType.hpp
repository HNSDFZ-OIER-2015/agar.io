//
// Copyright 2015 riteme
//

#ifndef RILOG_LOG_TYPE_HPP_
#define RILOG_LOG_TYPE_HPP_

#include <functional>

namespace ri {

enum class LogType {
    Unknown = 0,
    Debug = 1,
    Trace = 2,
    Info = 3,
    Warning = 4,
    Error = 5,
    Fatal = 6
};  // enum class LogType

}  // namespace ri

namespace std {

template <>
struct hash<ri::LogType> {
    auto operator()(const ri::LogType &type) const -> size_t {
        return static_cast<size_t>(type);
    }
};  // struct hash<LogType>

}  // namespace std

#endif  // RILOG_LOG_TYPE_HPP_
