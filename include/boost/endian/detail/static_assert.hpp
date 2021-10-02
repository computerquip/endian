#ifndef BOOST_ENDIAN_DETAIL_STATIC_ASSERT_HPP_INCLUDED
#define BOOST_ENDIAN_DETAIL_STATIC_ASSERT_HPP_INCLUDED

// Copyright 2021 Zachary Lund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

// Because C++ decided to require a literal in their static assert,
// we can't use it interchangeably with Boost.StaticAssert.
// So we just emulate that ourselves.
#if defined(BOOST_ENDIAN_USE_STDCXX11)
# define BOOST_ENDIAN_STATIC_ASSERT(expr) static_assert(expr, #expr)
#else
# define BOOST_ENDIAN_STATIC_ASSERT(expr) BOOST_STATIC_ASSERT(expr)
#endif

#endif  // BOOST_ENDIAN_DETAIL_IS_SCOPED_ENUM_HPP_INCLUDED
