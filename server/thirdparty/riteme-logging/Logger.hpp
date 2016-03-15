//
// Copyright 2015 riteme
//

#ifndef RILOG_LOGGER_HPP_
#define RILOG_LOGGER_HPP_

#include <string>
#include <vector>
#include <stdexcept>

#include "thirdparty/cppformat.hpp"

#include "LogType.hpp"
#include "Formatter.hpp"
#include "Writer.hpp"

namespace ri {

class Logger final {
 public:
    Logger() = default;
    ~Logger() = default;

    /**
     * 绑定格式器
     * @param formatter 实例指针
     * @remark:
     *     如果为nullptr，则使用默认的格式器
     */
    void AttachFormatter(Formatter *formatter) {
        if (formatter == nullptr) {
            m_pFormatter = &m_default_formatter;
        } else {
            m_pFormatter = formatter;
        }
    }

    /**
     * 绑定写入器
     * @param writer 写入器实例指针
     */
    void AttachWriter(Writer *writer) {
        m_writers.push_back(writer);
    }

    /**
     * 解除格式器，将使用默认的格式器
     */
    void DetachFormatter() {
        m_pFormatter = &m_default_formatter;
    }

    /**
     * 撤离指定的写入器
     * @param writer 实例指针
     * @remark:
     *     如果该写入器没有和日志器绑定，则直接忽略
     */
    void DetachWriter(const Writer *writer) {
        auto iter = std::find(m_writers.begin(), m_writers.end(), writer);

        if (iter != m_writers.end()) {
            m_writers.erase(iter);
        }
    }

    /**
     * 解除所有写入器的绑定
     */
    void DetachAllWriter() {
        m_writers.clear();
    }

    /**
     * 获取当前格式器的引用
     * @return 引用
     */
    auto GetFormatter() -> Formatter & {
        return *m_pFormatter;
    }

    /**
     * 设置日志最低级别
     * @param level 最低级别
     */
    void SetLevel(const LogType &level) {
        m_level = level;
    }

    /**
     * 获取当前日志最低级别
     * @return 最低级别
     */
    auto GetLevel() const -> LogType {
        return m_level;
    }

    /**
     * 指示当前日志器是否有效
     * @return 如果绑定了格式器和写入器就有效
     */
    auto IsValid() const -> bool {
        return m_pFormatter != nullptr and m_writers.size() > 0;
    }

    /**
     * 日志输出
     * @param type      日志类别
     * @param formatted 格式化过的日志信息
     * @param args      格式化的参数
     */
    template <typename... Args>
    void Log(const LogType &type, const std::string &formatted,
             const Args &... args) {
        if (!IsValid()) {
            throw std::runtime_error("Formatter or Writer not valid.");
        }

        if (static_cast<int>(type) < static_cast<int>(m_level)) {
            return;
        }

        Message message;
        message.LogType = type;
        time(&message.RawTime);
        message.Text = fmt::format(formatted, args...);
        message.ThreadID = std::this_thread::get_id();

        std::string buffer = m_pFormatter->Format(message);

        for (auto writer : m_writers) {
            std::lock_guard<std::mutex> guard(writer->_GetInnerLock());
            writer->Write(buffer);
            writer->Write("\n");
            writer->Flush();
        }  // foreach in m_writers
    }

    /**
     * 调试日志
     */
    template <typename... Args>
    void Debug(const std::string &formatted, const Args &... args) {
        Log(LogType::Debug, formatted, args...);
    }

    /**
     * 追溯日志
     */
    template <typename... Args>
    void Trace(const std::string &formatted, const Args &... args) {
        Log(LogType::Trace, formatted, args...);
    }

    /**
     * 普通消息日志
     */
    template <typename... Args>
    void Info(const std::string &formatted, const Args &... args) {
        Log(LogType::Info, formatted, args...);
    }

    /**
     * 警告级别日志
     */
    template <typename... Args>
    void Warning(const std::string &formatted, const Args &... args) {
        Log(LogType::Warning, formatted, args...);
    }

    /**
     * 错误日志
     */
    template <typename... Args>
    void Error(const std::string &formatted, const Args &... args) {
        Log(LogType::Error, formatted, args...);
    }

    /**
     * 严重错误日志
     */
    template <typename... Args>
    void Fatal(const std::string &formatted, const Args &... args) {
        Log(LogType::Fatal, formatted, args...);
    }

 private:
    LogType m_level = LogType::Info;
    DefaultFormatter m_default_formatter;
    Formatter *m_pFormatter = &m_default_formatter;
    std::vector<Writer *> m_writers;
};  // class Logger

}  // namespace ri

#endif  // RILOG_LOGGER_HPP_
