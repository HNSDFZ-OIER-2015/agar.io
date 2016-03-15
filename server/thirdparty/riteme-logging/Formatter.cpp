//
// Copyright 2015 riteme
//

#include "Formatter.hpp"

namespace ri {

Formatter::~Formatter() = default;

void Formatter::SetConfig(FormatConfig *config) {
    if (config == nullptr) {
        m_pConfig = &m_default;
    } else {
        m_pConfig = config;
    }
}

auto Formatter::GetConfig() const -> FormatConfig {
    return *m_pConfig;
}

void Formatter::SetDateFormat(const std::string &format) {
    m_pConfig->DateFormat = format;
}

void Formatter::SetDateBufferSize(const std::size_t size) {
    m_pConfig->DateBufferSize = size;
}

void Formatter::SetMessageFormat(const std::string &format) {
    m_pConfig->MessageFormat = format;
}

void Formatter::SetFieldName(const std::string &name) {
    m_pConfig->FieldName = name;
}

void Formatter::SetThreadName(const std::thread::id &id,
                              const std::string &name) {
    m_pConfig->SetThreadName(id, name);
}

void Formatter::SetCurrentThreadName(const std::string &name) {
    m_pConfig->SetCurrentThreadName(name);
}

}  // namespace ri
