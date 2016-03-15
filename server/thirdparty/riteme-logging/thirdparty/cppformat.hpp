#ifndef THRIDPARTY_CPPFORMAT_HPP_
#define THRIDPARTY_CPPFORMAT_HPP_

#define FMT_HEADER_ONLY

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#pragma clang diagnostic ignored "-Wheader-hygiene"
#endif  // IFDEF __clang__

#include "cppformat/format.h"

using namespace fmt::literals;

#ifdef __clang__
#pragma clang diagnostic pop
#endif  // IFDEF __clang__

#undef FMT_HEADER_ONLY

#endif  // THRIDPARTY_CPPFORMAT_HPP_
