#include "thirdparty/logging.hpp"

int main(int /*argc*/, char * /*argv*/ []) {
    LOG.RegisterThread("Main Thread");

    return 0;
}  // function main
