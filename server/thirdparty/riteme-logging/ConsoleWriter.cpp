//
// Copyright 2015 riteme
//

#include <iostream>

#include "ConsoleWriter.hpp"

namespace ri {
ConsoleWriter::ConsoleWriter() = default;
ConsoleWriter::~ConsoleWriter() = default;

void ConsoleWriter::Write(const std::string &buffer) {
    std::cout << buffer;
}

void ConsoleWriter::Flush() {
    std::cout.flush();
}

void ConsoleWriter::SetSync(const bool flag) {
    std::ios::sync_with_stdio(flag);
}

}  // namespace ri
