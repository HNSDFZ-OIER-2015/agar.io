//
// Copyright 2016 riteme
// Header file for Boost.ASIO
//

#ifndef AGAR_SERVER_3RD_BOOST_ASIO_HPP_
#define AGAR_SERVER_3RD_BOOST_ASIO_HPP_

#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

/**
 * Get IPv4 localhost.
 * @return TCP localhost.
 */
inline auto GetLocalhostV4() -> tcp {
    return tcp::v4();
}

/**
 * Get IPv6 localhost.
 * @return TCP localhost.
 */
inline auto GetLocalhostV6() -> tcp {
    return tcp::v6();
}

#endif  // AGAR_SERVER_3RD_BOOST_ASIO_HPP_
