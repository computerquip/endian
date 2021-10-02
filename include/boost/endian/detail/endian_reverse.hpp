#ifndef BOOST_ENDIAN_DETAIL_ENDIAN_REVERSE_HPP_INCLUDED
#define BOOST_ENDIAN_DETAIL_ENDIAN_REVERSE_HPP_INCLUDED

// Copyright 2019, 2020 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/endian/detail/cstdint.hpp>
#include <boost/endian/detail/integral_by_size.hpp>
#include <boost/endian/detail/intrinsic.hpp>
#include <boost/endian/detail/static_assert.hpp>
#include <boost/endian/detail/type_traits.hpp>
#include <cstddef>
#include <cstring>

#if defined(BOOST_ENDIAN_NO_INTRINSICS)
# if (__cplusplus < 201402L)
#  define BOOST_ENDIAN_CONSTEXPR
# else
#  define BOOST_ENDIAN_CONSTEXPR constexpr
# endif
#else
# if defined(BOOST_ENDIAN_CONSTEXPR_INTRINSICS)
#  define BOOST_ENDIAN_CONSTEXPR constexpr
# else
#  define BOOST_ENDIAN_CONSTEXPR
# endif
#endif

namespace boost
{
namespace endian
{
namespace detail
{

//  -- portable approach suggested by tymofey, with avoidance of undefined behavior
//     as suggested by Giovanni Piero Deretta, with a further refinement suggested
//     by Pyry Jahkola.
//  -- intrinsic approach suggested by reviewers, and by David Stone, who provided
//     his Boost licensed macro implementation (detail/intrinsic.hpp)

inline uint8_t constexpr endian_reverse_impl( uint8_t x ) noexcept
{
    return x;
}

inline uint16_t BOOST_ENDIAN_CONSTEXPR endian_reverse_impl( uint16_t x ) noexcept
{
#ifdef BOOST_ENDIAN_NO_INTRINSICS

    return (x << 8) | (x >> 8);

#else

    return BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_2(x);

#endif
}

inline uint32_t BOOST_ENDIAN_CONSTEXPR endian_reverse_impl( uint32_t x ) noexcept
{
#ifdef BOOST_ENDIAN_NO_INTRINSICS

    uint32_t step16 = x << 16 | x >> 16;
    return ((step16 << 8) & 0xff00ff00) | ((step16 >> 8) & 0x00ff00ff);

#else

    return BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_4(x);

#endif
}

inline uint64_t BOOST_ENDIAN_CONSTEXPR endian_reverse_impl( uint64_t x ) noexcept
{
#ifdef BOOST_ENDIAN_NO_INTRINSICS

    uint64_t step32 = x << 32 | x >> 32;
    uint64_t step16 = (step32 & 0x0000FFFF0000FFFFULL) << 16 | (step32 & 0xFFFF0000FFFF0000ULL) >> 16;
    return (step16 & 0x00FF00FF00FF00FFULL) << 8 | (step16 & 0xFF00FF00FF00FF00ULL) >> 8;

#else

    return BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_8(x);

# endif
}

#if defined(BOOST_HAS_INT128)

inline uint128_type BOOST_ENDIAN_CONSTEXPR endian_reverse_impl( uint128_type x ) noexcept
{
    return endian_reverse_impl( static_cast<uint64_t>( x >> 64 ) ) |
        static_cast<uint128_type>( endian_reverse_impl( static_cast<uint64_t>( x ) ) ) << 64;
}

#endif

// is_endian_reversible

template<class T> struct is_endian_reversible: integral_constant<bool,
    (is_integral<T>::value && !is_same<T, bool>::value) || is_scoped_enum<T>::value>
{
};

// is_endian_reversible_inplace

template<class T> struct is_endian_reversible_inplace: integral_constant<bool,
    is_integral<T>::value || is_enum<T>::value || is_same<T, float>::value || is_same<T, double>::value>
{
};

} // namespace detail

// Requires:
//   T is non-bool integral or scoped enumeration type

template<class T> inline BOOST_ENDIAN_CONSTEXPR
    typename detail::enable_if< !detail::is_class<T>::value, T >::type
    endian_reverse( T x ) noexcept
{
    BOOST_ENDIAN_STATIC_ASSERT( detail::is_endian_reversible<T>::value );

    typedef typename detail::integral_by_size< sizeof(T) >::type uintN_t;

    return static_cast<T>( detail::endian_reverse_impl( static_cast<uintN_t>( x ) ) );
}

// Requires:
//   T is integral, enumeration, float or double

template<class T> inline
    typename detail::enable_if< !detail::is_class<T>::value >::type
    endian_reverse_inplace( T & x ) noexcept
{
    BOOST_ENDIAN_STATIC_ASSERT( detail::is_endian_reversible_inplace<T>::value );

    typename detail::integral_by_size< sizeof(T) >::type x2;

    std::memcpy( &x2, &x, sizeof(T) );

    x2 = detail::endian_reverse_impl( x2 );

    std::memcpy( &x, &x2, sizeof(T) );
}

// Default implementation for user-defined types

template<class T> inline
    typename detail::enable_if< detail::is_class<T>::value >::type
    endian_reverse_inplace( T & x ) noexcept
{
    x = endian_reverse( x );
}

// endian_reverse_inplace for arrays

template<class T, std::size_t N>
inline void endian_reverse_inplace( T (&x)[ N ] ) noexcept
{
    for( std::size_t i = 0; i < N; ++i )
    {
        endian_reverse_inplace( x[i] );
    }
}

} // namespace endian
} // namespace boost

#endif  // BOOST_ENDIAN_DETAIL_ENDIAN_REVERSE_HPP_INCLUDED
