//  boost/endian/arithmetic.hpp  -------------------------------------------------------//

//  (C) Copyright Darin Adler 2000
//  (C) Copyright Beman Dawes 2006, 2009, 2014
//  (C) Copyright Peter Dimov 2019

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

//--------------------------------------------------------------------------------------//

//  Original design developed by Darin Adler based on classes developed by Mark
//  Borgerding. Four original class templates were combined into a single endian
//  class template by Beman Dawes, who also added the unrolled_byte_loops sign
//  partial specialization to correctly extend the sign when cover integer size
//  differs from endian representation size.

// TODO: When a compiler supporting constexpr becomes available, try possible uses.

#ifndef BOOST_ENDIAN_ARITHMETIC_HPP
#define BOOST_ENDIAN_ARITHMETIC_HPP

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable:4365)  // conversion ... signed/unsigned mismatch
#endif

#include <boost/endian/buffers.hpp>
#include <boost/endian/detail/cstdint.hpp>
#include <boost/endian/detail/static_assert.hpp>
#include <iosfwd>
#include <climits>

#if defined(BOOST_BORLANDC) || defined(BOOST_CODEGEARC)
# pragma pack(push, 1)
#endif

# if CHAR_BIT != 8
#   error Platforms with CHAR_BIT != 8 are not supported
# endif

# ifdef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#   define BOOST_ENDIAN_DEFAULT_CONSTRUCT {}          // C++03
# else
#   define BOOST_ENDIAN_DEFAULT_CONSTRUCT = default;  // C++0x
# endif

# ifndef BOOST_ENDIAN_EXPLICIT_CTORS
#   define BOOST_ENDIAN_EXPLICIT_OPT
# else
#   define BOOST_ENDIAN_EXPLICIT_OPT explicit
# endif

//----------------------------------  synopsis  ----------------------------------------//

namespace boost
{
namespace endian
{

  template <enum order Order, class T, std::size_t n_bits,
    enum align Align = align::no>
      class endian_arithmetic;

  // big endian signed integer aligned types
  typedef endian_arithmetic<order::big, int8_t, 8, align::yes>        big_int8_at;
  typedef endian_arithmetic<order::big, int16_t, 16, align::yes>      big_int16_at;
  typedef endian_arithmetic<order::big, int32_t, 32, align::yes>      big_int32_at;
  typedef endian_arithmetic<order::big, int64_t, 64, align::yes>      big_int64_at;

  // big endian unsigned integer aligned types
  typedef endian_arithmetic<order::big, uint8_t, 8, align::yes>       big_uint8_at;
  typedef endian_arithmetic<order::big, uint16_t, 16, align::yes>     big_uint16_at;
  typedef endian_arithmetic<order::big, uint32_t, 32, align::yes>     big_uint32_at;
  typedef endian_arithmetic<order::big, uint64_t, 64, align::yes>     big_uint64_at;

  // little endian signed integer aligned types
  typedef endian_arithmetic<order::little, int8_t, 8, align::yes>     little_int8_at;
  typedef endian_arithmetic<order::little, int16_t, 16, align::yes>   little_int16_at;
  typedef endian_arithmetic<order::little, int32_t, 32, align::yes>   little_int32_at;
  typedef endian_arithmetic<order::little, int64_t, 64, align::yes>   little_int64_at;

  // little endian unsigned integer aligned types
  typedef endian_arithmetic<order::little, uint8_t, 8, align::yes>    little_uint8_at;
  typedef endian_arithmetic<order::little, uint16_t, 16, align::yes>  little_uint16_at;
  typedef endian_arithmetic<order::little, uint32_t, 32, align::yes>  little_uint32_at;
  typedef endian_arithmetic<order::little, uint64_t, 64, align::yes>  little_uint64_at;

  // aligned floating point types
  typedef endian_arithmetic<order::big, float, 32, align::yes>        big_float32_at;
  typedef endian_arithmetic<order::big, double, 64, align::yes>       big_float64_at;
  typedef endian_arithmetic<order::little, float, 32, align::yes>     little_float32_at;
  typedef endian_arithmetic<order::little, double, 64, align::yes>    little_float64_at;

  // aligned native endian typedefs are not provided because
  // <cstdint> types are superior for this use case

  // big endian signed integer unaligned types
  typedef endian_arithmetic<order::big, int_least8_t, 8>        big_int8_t;
  typedef endian_arithmetic<order::big, int_least16_t, 16>      big_int16_t;
  typedef endian_arithmetic<order::big, int_least32_t, 24>      big_int24_t;
  typedef endian_arithmetic<order::big, int_least32_t, 32>      big_int32_t;
  typedef endian_arithmetic<order::big, int_least64_t, 40>      big_int40_t;
  typedef endian_arithmetic<order::big, int_least64_t, 48>      big_int48_t;
  typedef endian_arithmetic<order::big, int_least64_t, 56>      big_int56_t;
  typedef endian_arithmetic<order::big, int_least64_t, 64>      big_int64_t;

  // big endian unsigned integer unaligned types
  typedef endian_arithmetic<order::big, uint_least8_t, 8>       big_uint8_t;
  typedef endian_arithmetic<order::big, uint_least16_t, 16>     big_uint16_t;
  typedef endian_arithmetic<order::big, uint_least32_t, 24>     big_uint24_t;
  typedef endian_arithmetic<order::big, uint_least32_t, 32>     big_uint32_t;
  typedef endian_arithmetic<order::big, uint_least64_t, 40>     big_uint40_t;
  typedef endian_arithmetic<order::big, uint_least64_t, 48>     big_uint48_t;
  typedef endian_arithmetic<order::big, uint_least64_t, 56>     big_uint56_t;
  typedef endian_arithmetic<order::big, uint_least64_t, 64>     big_uint64_t;

  // little endian signed integer unaligned types
  typedef endian_arithmetic<order::little, int_least8_t, 8>     little_int8_t;
  typedef endian_arithmetic<order::little, int_least16_t, 16>   little_int16_t;
  typedef endian_arithmetic<order::little, int_least32_t, 24>   little_int24_t;
  typedef endian_arithmetic<order::little, int_least32_t, 32>   little_int32_t;
  typedef endian_arithmetic<order::little, int_least64_t, 40>   little_int40_t;
  typedef endian_arithmetic<order::little, int_least64_t, 48>   little_int48_t;
  typedef endian_arithmetic<order::little, int_least64_t, 56>   little_int56_t;
  typedef endian_arithmetic<order::little, int_least64_t, 64>   little_int64_t;

  // little endian unsigned integer unaligned types
  typedef endian_arithmetic<order::little, uint_least8_t, 8>    little_uint8_t;
  typedef endian_arithmetic<order::little, uint_least16_t, 16>  little_uint16_t;
  typedef endian_arithmetic<order::little, uint_least32_t, 24>  little_uint24_t;
  typedef endian_arithmetic<order::little, uint_least32_t, 32>  little_uint32_t;
  typedef endian_arithmetic<order::little, uint_least64_t, 40>  little_uint40_t;
  typedef endian_arithmetic<order::little, uint_least64_t, 48>  little_uint48_t;
  typedef endian_arithmetic<order::little, uint_least64_t, 56>  little_uint56_t;
  typedef endian_arithmetic<order::little, uint_least64_t, 64>  little_uint64_t;

  // native endian signed integer unaligned types
  typedef endian_arithmetic<order::native, int_least8_t, 8>     native_int8_t;
  typedef endian_arithmetic<order::native, int_least16_t, 16>   native_int16_t;
  typedef endian_arithmetic<order::native, int_least32_t, 24>   native_int24_t;
  typedef endian_arithmetic<order::native, int_least32_t, 32>   native_int32_t;
  typedef endian_arithmetic<order::native, int_least64_t, 40>   native_int40_t;
  typedef endian_arithmetic<order::native, int_least64_t, 48>   native_int48_t;
  typedef endian_arithmetic<order::native, int_least64_t, 56>   native_int56_t;
  typedef endian_arithmetic<order::native, int_least64_t, 64>   native_int64_t;

  // native endian unsigned integer unaligned types
  typedef endian_arithmetic<order::native, uint_least8_t, 8>    native_uint8_t;
  typedef endian_arithmetic<order::native, uint_least16_t, 16>  native_uint16_t;
  typedef endian_arithmetic<order::native, uint_least32_t, 24>  native_uint24_t;
  typedef endian_arithmetic<order::native, uint_least32_t, 32>  native_uint32_t;
  typedef endian_arithmetic<order::native, uint_least64_t, 40>  native_uint40_t;
  typedef endian_arithmetic<order::native, uint_least64_t, 48>  native_uint48_t;
  typedef endian_arithmetic<order::native, uint_least64_t, 56>  native_uint56_t;
  typedef endian_arithmetic<order::native, uint_least64_t, 64>  native_uint64_t;

  // unaligned floating point types
  typedef endian_arithmetic<order::big, float, 32, align::no>        big_float32_t;
  typedef endian_arithmetic<order::big, double, 64, align::no>       big_float64_t;
  typedef endian_arithmetic<order::little, float, 32, align::no>     little_float32_t;
  typedef endian_arithmetic<order::little, double, 64, align::no>    little_float64_t;
  typedef endian_arithmetic<order::native, float, 32, align::no>     native_float32_t;
  typedef endian_arithmetic<order::native, double, 64, align::no>    native_float64_t;

//----------------------------------  end synopsis  ------------------------------------//

template <enum order Order, class T, std::size_t n_bits,
    enum align Align>
class endian_arithmetic
{
private:

    typedef endian_buffer<Order, T, n_bits, Align> buffer_type;

#ifdef BOOST_ENDIAN_NO_CTORS
public:
#else
private:
#endif

    buffer_type buf_;

public:

    typedef T value_type;

#ifndef BOOST_ENDIAN_NO_CTORS

    endian_arithmetic() BOOST_ENDIAN_DEFAULT_CONSTRUCT

    BOOST_ENDIAN_EXPLICIT_OPT endian_arithmetic( T val ) noexcept: buf_( val )
    {
    }

#endif

    endian_arithmetic& operator=( T val ) noexcept
    {
        buf_ = val;
        return *this;
    }

    value_type value() const noexcept
    {
        return buf_.value();
    }

    unsigned char const * data() const noexcept
    {
        return buf_.data();
    }

    unsigned char * data() noexcept
    {
        return buf_.data();
    }

    operator value_type() const noexcept
    {
        return this->value();
    }

    operator buffer_type& () noexcept
    {
        return buf_;
    }

    operator buffer_type const& () noexcept
    {
        return buf_;
    }

    // operators

    T operator+() const noexcept
    {
        return this->value();
    }

    endian_arithmetic& operator+=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() + y );
        return *this;
    }

    endian_arithmetic& operator-=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() - y );
        return *this;
    }

    endian_arithmetic& operator*=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() * y );
        return *this;
    }

    endian_arithmetic& operator/=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() / y );
        return *this;
    }

    endian_arithmetic& operator%=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() % y );
        return *this;
    }

    endian_arithmetic& operator&=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() & y );
        return *this;
    }

    endian_arithmetic& operator|=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() | y );
        return *this;
    }

    endian_arithmetic& operator^=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() ^ y );
        return *this;
    }

    endian_arithmetic& operator<<=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() << y );
        return *this;
    }

    endian_arithmetic& operator>>=( T y ) noexcept
    {
        *this = static_cast<T>( this->value() >> y );
        return *this;
    }

    endian_arithmetic& operator++() noexcept
    {
        *this += 1;
        return *this;
    }

    endian_arithmetic& operator--() noexcept
    {
        *this -= 1;
        return *this;
    }

    endian_arithmetic operator++(int) noexcept
    {
        endian_arithmetic tmp( *this );
        *this += 1;
        return tmp;
    }

    endian_arithmetic operator--(int) noexcept
    {
        endian_arithmetic tmp( *this );
        *this -= 1;
        return tmp;
    }

    template<class Ch, class Tr>
    friend std::basic_ostream<Ch, Tr>&
    operator<<( std::basic_ostream<Ch, Tr>& os, endian_arithmetic const& x )
    {
        return os << x.value();
    }

    template<class Ch, class Tr>
    friend std::basic_istream<Ch, Tr>&
    operator>>( std::basic_istream<Ch, Tr>& is, endian_arithmetic& x )
    {
        T i;

        if( is >> i )
        {
            x = i;
        }

        return is;
    }
};

} // namespace endian
} // namespace boost

#if defined(BOOST_BORLANDC) || defined(BOOST_CODEGEARC)
# pragma pack(pop)
#endif

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // BOOST_ENDIAN_ARITHMETIC_HPP
