//
// Copyright 2015 riteme
//

#include <stdexcept>

#include "FileWriter.hpp"

namespace ri {

FileWriter::FileWriter() = default;

FileWriter::FileWriter(const std::string &file) {
    Open(file);
}

FileWriter::~FileWriter() = default;

void FileWriter::Write(const std::string &buffer) {
    if (!IsOpened()) {
        throw std::runtime_error("FileWriter not valid: no file opened.");
    }

    m_file << buffer;
}

void FileWriter::Flush() {
    m_file.flush();
}

void FileWriter::Open(const std::string &file) {
    if (IsOpened()) {
        Close();
    }

    m_file.open(file);

    if (!IsOpened()) {
        throw std::runtime_error("Failed to open the file.");
    }
}

void FileWriter::Close() {
    if (IsOpened()) {
        m_file.close();
    }
}

auto FileWriter::IsOpened() const -> bool {
    return m_file.is_open();
}

}  // namespace ri
