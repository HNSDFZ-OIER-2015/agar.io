//
// Copyright 2016 riteme
// Header file for logging system
// MARCOS:
//     LOGGING_USE_COLORFUL_OUTPUT: Indicate whether uses colorful log output.
//

#ifndef AGAR_SERVER_3RD_LOGGING_HPP_
#define AGAR_SERVER_3RD_LOGGING_HPP_

#include "riteme-logging/Logging.hpp"
#include "riteme-logging/LogTypeName.hpp"
#include "riteme-logging/FileWriter.hpp"

#include <string>

namespace logging = ri;

/**
 * Default log file.
 */
#define SERVER_DEFAULT_LOG_FILE "server.log"

/**
 * Log types.
 */
#define LOG_UNKNOWN logging::LogType::Unknown
#define LOG_DEBUG logging::LogType::Debug
#define LOG_TRACE logging::LogType::Trace
#define LOG_INFO logging::LogType::Info
#define LOG_WARN logging::LogType::Warning
#define LOG_ERROR logging::LogType::Error
#define LOG_FATAL logging::LogType::Fatal

/**
 * The logger of this agar server.
 */
class ServerLogger {
 public:
    /**
     * Construct the logger.
     */
    ServerLogger(const std::string &logFile) {
#ifdef LOGGING_USE_COLORFUL_OUTPUT
        _format.SetMessageFormat(
            "\033[32m[{date}]\033[0m [{thread}][{field}] {LOGTYPE} "
            "\033[37m{text}\033[0m");
#else
        _format.SetMessageFormat(
            "[{date}] [{thread}][{field}] [{LOGTYPE}] "
            "{text}");
#endif  // IFDEF LOGGING_USE_COLORFUL_OUTPUT

        _format.SetFieldName("Server");

        _logger.AttachFormatter(&_format);
        _logger.AttachWriter(&_writer);

#ifndef NDEBUG
        _logger.SetLevel(logging::LogType::Unknown);
#endif  // IFNDEF NDEBUG

        if (not logFile.empty()) {
            _file.Open(logFile);

            if (_file.IsOpened()) {
                _logger.AttachWriter(&_file);
            }
        }
    }

    /**
     * Destroy the logger.
     */
    ~ServerLogger() = default;

    /**
     * Set current thread's name.
     * @param threadName Thread name.
     */
    void RegisterThread(const std::string &threadName) {
        _format.SetCurrentThreadName(threadName);
    }

    /**
     * Log in.
     * @param type Log type.
     * @param text Log message.
     * @param args Format arguments.
     */
    template <typename TText, typename... Args>
    void operator()(const logging::LogType type, const TText &text,
                    const Args &... args) {
        switch (type) {
            case logging::LogType::Unknown:
                _logger.Log(logging::LogType::Unknown, text, args...);
                break;

            case logging::LogType::Debug:
#ifndef NDEBUG
                _logger.Debug(text, args...);
#endif  // IFNDEF NDEBUG
                break;

            case logging::LogType::Trace: _logger.Trace(text, args...); break;

            case logging::LogType::Info: _logger.Info(text, args...); break;

            case logging::LogType::Warning:
                _logger.Warning(text, args...);
                break;

            case logging::LogType::Error: _logger.Error(text, args...); break;

            case logging::LogType::Fatal: _logger.Fatal(text, args...); break;
        }  // switch to type
    }

 private:
    logging::Logger _logger;
    logging::CppformatFormatter _format;
    logging::ConsoleWriter _writer;
    logging::FileWriter _file;
};  // class ServerLogger

static ServerLogger LOG(SERVER_DEFAULT_LOG_FILE);

#endif  // AGAR_SERVER_3RD_LOGGING_HPP_
