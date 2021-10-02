#ifndef BOOST_ENDIAN_DETAIL_TYPE_TRAITS_HPP_INCLUDED
#define BOOST_ENDIAN_DETAIL_TYPE_TRAITS_HPP_INCLUDED

// Copyright 2020 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#if defined(BOOST_ENDIAN_USE_STDCXX11)
# include <type_traits>
#else
# include <boost/type_traits/is_signed.hpp>
# include <boost/type_traits/is_integral.hpp>
# include <boost/type_traits/is_enum.hpp>
#endif

namespace boost
{
namespace endian
{
namespace detail
{

#if defined(BOOST_ENDIAN_USE_STDCXX11)
template<typename T> struct is_signed: std::is_signed<T>{};
template<typename T> struct is_integral: std::is_integral<T>{};
template<typename T> struct is_enum: std::is_enum<T>{};
template<typename T> struct is_class: std::is_class<T>{};
template<typename T> struct is_array: std::is_array<T>{};
template<typename T> struct is_trivially_copyable: std::is_trivially_copyable<T>{};
template<typename From, typename To> struct is_convertible: std::is_convertible<From, To>{};
template<typename T, T v> struct integral_constant: std::integral_constant<T, v>{};
template<bool B, typename T, typename F> struct conditional: std::conditional<B, T, F>{};
template<typename T, typename U> struct is_same: std::is_same<T, U>{};
template<bool B, typename T = void> struct enable_if: std::enable_if<B, T>{};
typedef std::false_type false_type;
typedef std::true_type true_type;
#else
template<typename T> struct is_signed: boost::is_signed<T>{};
template<typename T> struct is_integral: boost::is_integral<T>{};
template<typename T> struct is_enum: boost::is_enum<T>{};
template<typename T> struct is_array: boost::is_array<T>{};
template<typename T> struct is_trivially_copyable: boost::is_trivially_copyable<T>{};
template<typename From, typename To> struct is_convertible: boost::is_convertible<From, To>{};
template<typename T, T v> struct integral_constant: boost::integral_constant<T, v>{};
template<bool B, typename T, typename F> struct conditional: boost::conditional<B, T, F>{};
template<typename T, typename U> struct is_same: boost::is_same<T, U>{};
template<bool B, typename T = void> struct enable_if: boost::enable_if<B, T>{};
template<typename T> struct is_class: boost::is_class<T>{};
typedef boost::false_type false_type;
typedef boost::true_type true_type;
#endif

template<class T> struct negation: integral_constant<bool, !T::value>{};

template<class T> struct is_scoped_enum:
    conditional<
        is_enum<T>::value,
        negation< is_convertible<T, int> >,
        false_type
    >::type
{
};

} // namespace detail
} // namespace endian
} // namespace boost

#endif  // BOOST_ENDIAN_DETAIL_IS_SCOPED_ENUM_HPP_INCLUDED
