//
// Copyright 2016 riteme
//

#include <cstdio>

#include "rapidjson.hpp"

auto LoadDocument(const std::string &file) -> json::Document {
    char buffer[JSON_READ_BUFFER_SIZE];

#ifdef __WINDOWS__
    FILE *fp = fopen(file.data(), "rb");
#else
    FILE *fp = fopen(file.data(), "r");
#endif  // IFDEF __WINDOWS__

    json::FileReadStream stream(fp, buffer, sizeof(buffer));

    json::Document document;
    document.ParseStream(stream);

    return document;
}
