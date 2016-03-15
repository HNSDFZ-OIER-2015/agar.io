//
// Copyright 2015 riteme
//

#ifndef RILOG_FORMATTER_HPP_
#define RILOG_FORMATTER_HPP_

#include <string>

#include "Message.hpp"
#include "FormatConfig.hpp"

namespace ri {

class Formatter {
 public:
    virtual ~Formatter();

    virtual auto Format(const Message &message) const -> std::string = 0;

    /**
     * 指定配置信息
     * @param config 配置信息实例的指针
     */
    void SetConfig(FormatConfig *config);

    /**
     * 获取当前配置
     * @return 配置实例
     */
    auto GetConfig() const -> FormatConfig;

    ////////////////////////////////////////
    // 以下函数的注释参见FormatConfig.hpp中 //
    ////////////////////////////////////////

    /**
     * 设置日期格式
     */
    void SetDateFormat(const std::string &format);

    /**
     * 设置日期字符串缓冲大小
     */
    void SetDateBufferSize(const std::size_t size);

    /**
     * 设置日志格式
     */
    void SetMessageFormat(const std::string &format);

    /**
     * 设置当前区域名称
     */
    void SetFieldName(const std::string &name);

    /**
     * 设置线程名称
     */
    void SetThreadName(const std::thread::id &id, const std::string &name);

    /**
     * 设置当前线程名称
     */
    void SetCurrentThreadName(const std::string &name);

 protected:
    FormatConfig m_default;
    FormatConfig *m_pConfig = &m_default;
};  // class Formatter

}  // namespace ri

#endif  // RILOG_FORMATTER_HPP_
