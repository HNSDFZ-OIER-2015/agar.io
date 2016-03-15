#include "thirdparty/logging.hpp"

int main() {
    LOG.RegisterThread("Main Thread");
    LOG(LOG_UNKNOWN, "Unknown");
    LOG(LOG_DEBUG, "Debug");
    LOG(LOG_TRACE, "Trace");
    LOG(LOG_INFO, "Info");
    LOG(LOG_WARN, "Warning");
    LOG(LOG_ERROR, "Error");
    LOG(LOG_FATAL, "Fatal");

    return 0;
}  // function main
