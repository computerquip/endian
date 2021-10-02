#ifndef BOOST_ENDIAN_DETAIL_CSTDINT_HPP_INCLUDED
#define BOOST_ENDIAN_DETAIL_CSTDINT_HPP_INCLUDED

// Copyright 2021 Zachary Lund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#if defined(BOOST_ENDIAN_USE_STDCXX11)
# include <cstdint>
#else
# include <boost/cstdint.hpp>
#endif

namespace boost
{
namespace endian
{
namespace detail
{

#if defined(BOOST_ENDIAN_USE_STDCXX11)
typedef std::uint8_t uint8_t;
typedef std::uint16_t uint16_t;
typedef std::uint32_t uint32_t;
typedef std::uint64_t uint64_t;
typedef std::int8_t int8_t;
typedef std::int16_t int16_t;
typedef std::int32_t int32_t;
typedef std::int64_t int64_t;
#else
typedef boost::uint8_t uint8_t;
typedef boost::uint16_t uint16_t;
typedef boost::uint32_t uint32_t;
typedef boost::uint64_t uint64_t;
typedef boost::int8_t int8_t;
typedef boost::int16_t int16_t;
typedef boost::int32_t int32_t;
typedef boost::int64_t int64_t;
#endif

} // namespace detail
} // namespace endian
} // namespace boost

#endif  // BOOST_ENDIAN_DETAIL_IS_SCOPED_ENUM_HPP_INCLUDED
