//
// Copyright 2015 riteme
//

#ifndef RILOG_LOG_TYPE_NAME_HPP_
#define RILOG_LOG_TYPE_NAME_HPP_

#include <string>
#include <unordered_map>

#include "LogType.hpp"

namespace ri {

#ifdef LOGGING_USE_COLORFUL_OUTPUT
static std::unordered_map<LogType, std::string> LOGTYPE = {
    { LogType::Unknown, "\033[30m[UNKNOWN]\033[0m" },
    { LogType::Debug, "\033[33m[DEBUG]\033[0m" },
    { LogType::Trace, "\033[34m[TRACE]\033[0m" },
    { LogType::Info, "\033[32m[INFO]\033[0m " },
    { LogType::Warning, "\033[33m[WARN]\033[0m " },
    { LogType::Error, "\033[31m[ERROR]\033[0m" },
    { LogType::Fatal, "\033[35m[FATAL]\033[0m" }
};
#else
static std::unordered_map<LogType, std::string> LOGTYPE = {
    { LogType::Unknown, "UNKNOWN" },
    { LogType::Debug, "DEBUG" },
    { LogType::Trace, "TRACE" },
    { LogType::Info, "INFO " },
    { LogType::Warning, "WARN " },
    { LogType::Error, "ERROR" },
    { LogType::Fatal, "FATAL" }
};
#endif  // LOGGING_USE_COLORFUL_OUTPUT

static std::unordered_map<LogType, std::string> logtype = {
    { LogType::Unknown, "Unknown" },
    { LogType::Debug, "Debug" },
    { LogType::Trace, "Trace" },
    { LogType::Info, "Info" },
    { LogType::Warning, "Warning" },
    { LogType::Error, "Error" },
    { LogType::Fatal, "Fatal" }
};

}  // namespace ri

#endif  // RILOG_LOG_TYPE_NAME_HPP_
