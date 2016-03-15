//
// Copyright 2015 riteme
//

#ifndef RILOG_CPPFORMAT_FORMATTER_HPP_
#define RILOG_CPPFORMAT_FORMATTER_HPP_

#include "Formatter.hpp"

namespace ri {

/**
 * 使用cppformat进行格式化
 */
class CppformatFormatter : public Formatter {
 public:
    CppformatFormatter();
    virtual ~CppformatFormatter();

    /**
     * 将消息格式化输出
     * @param  message 表示一条日志消息
     * @return         返回格式化后的字符串
     * @remark:
     *     如果没有设置线程的名称，则为"Unknown Thread"
     */
    virtual auto Format(const Message &message) const -> std::string;
};  // class CppformatFormatter

}  // namespace ri

#endif  // RILOG_CPPFORMAT_FORMATTER_HPP_
