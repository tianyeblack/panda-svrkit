
//******************************************************************************
// RCF - Remote Call Framework
//
// Copyright (c) 2005 - 2012, Delta V Software. All rights reserved.
// http://www.deltavsoft.com
//
// RCF is distributed under dual licenses - closed source or GPL.
// Consult your particular license for conditions of use.
//
// If you have not purchased a commercial license, you are using RCF 
// under GPL terms.
//
// Version: 2.0
// Contact: support <at> deltavsoft.com 
//
//******************************************************************************

#ifndef INCLUDE_RCF_UTIL_SPCOLLECTOR_HPP
#define INCLUDE_RCF_UTIL_SPCOLLECTOR_HPP

#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS) && !defined(__MINGW32__)

// this causes naming collisions for classes named "X" in RCF tests...
//#include <boost/config/../../libs/smart_ptr/src/sp_collector.cpp>

// exported from sp_collector.cpp
std::size_t find_unreachable_objects(bool report);
void free_unreachable_objects();

void checkNoCycles()
{
    std::cout <<  "Checking for shared_ptr cycles..." << std::endl;
    std::size_t count = find_unreachable_objects(true);
    if (count > 0)
    {
        std::cout <<  "Detected shared_ptr cycles! Unreachable objects: " << count << std::endl;
        abort();
    }
    else
    {
        std::cout <<  "No shared_ptr cycles detected." << std::endl;
    }
}

#else

void checkNoCycles()
{
    std::cout <<  "shared_ptr cycle detection not enabled." << std::endl;
}

#endif

#endif // ! INCLUDE_RCF_UTIL_SPCOLLECTOR_HPP
