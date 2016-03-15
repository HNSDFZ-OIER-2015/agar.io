//
// Copyright 2015 riteme
//

#ifndef RILOG_WRITER_HPP_
#define RILOG_WRITER_HPP_

#include <mutex>
#include <string>

namespace ri {

class Writer {
 public:
    virtual ~Writer();

    /**
     * 写入数据
     * @param buffer 数据
     */
    virtual void Write(const std::string &buffer) = 0;

    /**
     * 刷新缓冲
     */
    virtual void Flush() = 0;

 protected:
    friend class Logger;

    /**
     * 获取内部的锁（Mutex）
     * @return 返回锁
     */
    auto _GetInnerLock() -> std::mutex &;

    std::mutex m_innerLock;
};  // class Writer

}  // namespace ri

#endif  // RILOG_WRITER_HPP_
