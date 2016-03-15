//
// Copyright 2015 riteme
//

#include "Writer.hpp"

namespace ri {

Writer::~Writer() = default;

auto Writer::_GetInnerLock() -> std::mutex & {
    return m_innerLock;
}

}  // namespace ri
