//
// Copyright 2015 riteme
//

#ifndef RILOG_CONSOLE_WRITER_HPP
#define RILOG_CONSOLE_WRITER_HPP

#include "Writer.hpp"

namespace ri {

/**
 * 将日志类容写入控制台（通过std::cout）
 */
class ConsoleWriter : public Writer {
 public:
    ConsoleWriter();
    virtual ~ConsoleWriter();

    /**
     * 写入操作
     * @param buffer 要写入的类容
     */
    virtual void Write(const std::string &buffer);

    /**
     * 刷新缓冲，将缓冲中的类容展现出来
     */
    virtual void Flush();

    /**
     * 设置是否和C的缓冲同步
     * @param flag true表示同步，反之表示不同步
     */
    virtual void SetSync(const bool flag);
};  // class ConsoleWriter

}  // namespace ri

#endif  // RILOG_CONSOLE_WRITER_HPP
