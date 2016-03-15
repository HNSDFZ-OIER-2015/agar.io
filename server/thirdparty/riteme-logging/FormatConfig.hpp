//
// Copyright 2015 riteme
//

#ifndef RILOG_FORMAT_CONFIG_HPP_
#define RILOG_FORMAT_CONFIG_HPP_

#include <string>
#include <thread>
#include <unordered_map>

namespace ri {

struct FormatConfig {
    /**
     * 日期格式。
     * 将会用strftime进行格式化。
     */
    std::string DateFormat = "%F/%T";

    /**
     * 用strftime时的缓冲大小，包括结尾的空字符。
     */
    std::size_t DateBufferSize = 20;

    /**
     * 日志消息格式。
     * 使用cppformat进行格式化。
     * 格式字符窜中可以使用以下变量：
     *
     * * date:     表示日期，可用DateFormat来设定
     * * field:    表示程序的名称，例如Server, Client
     * * thread:   线程的名称，用FormatConfig中所设定的
     * * logtype:  小写的日志类别，如Debug, Info, Warning，可配置
     * * LOGTYPE:  大写的日志类别，如DEBUG, INFO, WARN，可配置
     * * text:     日志类容
     *
     * 变量名用打括号包含
     */
    std::string MessageFormat = "[{date}][{thread}][{LOGTYPE}] {text}";

    /**
     * 程序名称
     */
    std::string FieldName = "Unknown";

    /**
     * 线程名称表
     */
    std::unordered_map<std::thread::id, std::string> ThreadMap;

    /**
     * 设置线程的名称
     * @param id   线程ID
     * @param name 线程的名称
     */
    void SetThreadName(const std::thread::id &id, const std::string &name) {
        ThreadMap[id] = name;
    }

    /**
     * 设置当前线程的名称
     * @param name 名称
     */
    void SetCurrentThreadName(const std::string &name) {
        ThreadMap[std::this_thread::get_id()] = name;
    }
};  // struct FormatConfig

}  // namespace ri

#endif  // RILOG_FORMAT_CONFIG_HPP_
