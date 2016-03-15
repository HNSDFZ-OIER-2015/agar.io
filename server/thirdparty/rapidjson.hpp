//
// Copyright 2016 riteme
// Header file for Rapid JSON
//

#ifndef AGAR_SERVER_3RD_RAPID_JSON_HPP_
#define AGAR_SERVER_3RD_RAPID_JSON_HPP_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif  // IFDEF __clang__

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif  // IFDEF __clang__

#include <string>

namespace json = rapidjson;

#define JSON_READ_BUFFER_SIZE 65536U

/**
 * Load a JSON Document.
 * @param  file JSON file to load.
 * @return      A Rapid JSON Document.
 */
auto LoadDocument(const std::string &file) -> json::Document;

#endif  // AGAR_SERVER_3RD_RAPID_JSON_HPP_
