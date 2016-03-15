//
// Copyright 2015 riteme
//

#ifndef RILOG_FILE_WRITER_HPP_
#define RILOG_FILE_WRITER_HPP_

#include <fstream>

#include "Writer.hpp"

namespace ri {

/**
 * 向特定文件写入的写入器
 */
class FileWriter : public Writer {
 public:
    FileWriter();
    FileWriter(const std::string &file);
    virtual ~FileWriter();

    /**
     * 写入类容
     * @param buffer 类容
     */
    virtual void Write(const std::string &buffer);

    /**
     * 刷新文件缓冲
     */
    virtual void Flush();

    /**
     * 打开文件
     * @param file 文件路径
     * @remark:
     *     如果已经打开过文件了则先关闭
     */
    virtual void Open(const std::string &file);

    /**
     * 关闭文件
     */
    virtual void Close();

    /**
     * 指示是否打开了文件
     * @return true表示打开了
     */
    virtual auto IsOpened() const -> bool;

 private:
    std::ofstream m_file;
};  // class FileWriter

}  // namespace ri

#endif  // RILOG_FILE_WRITER_HPP_
