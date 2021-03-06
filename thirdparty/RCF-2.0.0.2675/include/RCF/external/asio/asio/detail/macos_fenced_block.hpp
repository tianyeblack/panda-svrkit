//
// detail/macos_fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_MACOS_FENCED_BLOCK_HPP
#define ASIO_DETAIL_MACOS_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/config.hpp"

#if defined(__MACH__) && defined(__APPLE__)

#include <libkern/OSAtomic.h>

#include "RCF/external/asio/asio/detail/push_options.hpp"

namespace asio {
namespace detail {

class macos_fenced_block
  : private noncopyable
{
public:
  // Constructor.
  macos_fenced_block()
  {
    OSMemoryBarrier();
  }

  // Destructor.
  ~macos_fenced_block()
  {
    OSMemoryBarrier();
  }
};

} // namespace detail
} // namespace asio

#include "RCF/external/asio/asio/detail/pop_options.hpp"

#endif // defined(__MACH__) && defined(__APPLE__)

#endif // ASIO_DETAIL_MACOS_FENCED_BLOCK_HPP
