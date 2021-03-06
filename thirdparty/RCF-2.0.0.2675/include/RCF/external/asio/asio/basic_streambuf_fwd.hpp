//
// basic_streambuf_fwd.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_BASIC_STREAMBUF_FWD_HPP
#define ASIO_BASIC_STREAMBUF_FWD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/config.hpp"

#if !defined(BOOST_NO_IOSTREAM)

#include <memory>

namespace asio {

template <typename Allocator = std::allocator<char> >
class basic_streambuf;

} // namespace asio

#endif // !defined(BOOST_NO_IOSTREAM)

#endif // ASIO_BASIC_STREAMBUF_FWD_HPP
