//
// Copyright 2015 riteme
//

#include <ctime>

#include "thirdparty/cppformat.hpp"

#include "LogTypeName.hpp"
#include "CppformatFormatter.hpp"

namespace ri {
CppformatFormatter::CppformatFormatter() = default;
CppformatFormatter::~CppformatFormatter() = default;

auto CppformatFormatter::Format(const Message &message) const -> std::string {
    struct tm *timeinfo = std::localtime(&message.RawTime);
    char buffer[m_pConfig->DateBufferSize];

    strftime(buffer, m_pConfig->DateBufferSize, m_pConfig->DateFormat.data(),
             timeinfo);
    std::string date = buffer;
    auto thread_name_iter = m_pConfig->ThreadMap.find(message.ThreadID);
    std::string thread_name;

    if (thread_name_iter == m_pConfig->ThreadMap.end()) {
        thread_name = "Unknown Thread";
    } else {
        thread_name = thread_name_iter->second;
    }

    return fmt::format(m_pConfig->MessageFormat,
                       "date"_a = date,
                       "field"_a = m_pConfig->FieldName,
                       "thread"_a = thread_name,
                       "logtype"_a = logtype[message.LogType],
                       "LOGTYPE"_a = LOGTYPE[message.LogType],
                       "text"_a = message.Text);
}

}  // namespace ri
