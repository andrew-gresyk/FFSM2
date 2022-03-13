﻿// FFSM2 (flat state machine for games and interactive applications)
// 1.4.0 (2022-03-13)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2022
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#define FFSM2_VERSION_MAJOR 1
#define FFSM2_VERSION_MINOR 4
#define FFSM2_VERSION_PATCH 0

#define FFSM2_VERSION (10000 * FFSM2_VERSION_MAJOR + 100 * FFSM2_VERSION_MINOR + FFSM2_VERSION_PATCH)

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef FFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif
#include <type_traits>		// is_base_of<>

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

#define FFSM2_UNUSED(x)

//------------------------------------------------------------------------------

#define FFSM2_ATTRIBUTE(A)								   FFSM2_ATTRIBUTE_##A()

#ifndef __has_cpp_attribute
	#define __has_cpp_attribute(x)											   0
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __has_cpp_attribute(fallthrough)
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()						 [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()				  [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()					[[gnu::fallthrough]]
#else
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __has_cpp_attribute(no_unique_address)
	#define FFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()			   [[no_unique_address]]
#else
	#define FFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()
#endif

//------------------------------------------------------------------------------

#define FFSM2_CONSTEXPR(A)								   FFSM2_CONSTEXPR_##A()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define FFSM2_CONSTEXPR_NO()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define FFSM2_CONSTEXPR_11()										   constexpr

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __cplusplus >= 201402L
	#define FFSM2_CONSTEXPR_14()									   constexpr
#else
	#define FFSM2_CONSTEXPR_14()										  inline
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __cplusplus >= 201703L
	#define FFSM2_CONSTEXPR_17()									   constexpr
#else
	#define FFSM2_CONSTEXPR_17()										  inline
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __cplusplus >= 202002L
	#define FFSM2_CONSTEXPR_20()									   constexpr
#else
	#define FFSM2_CONSTEXPR_20()										  inline
#endif

//------------------------------------------------------------------------------

#if defined _MSC_VER
	#define FFSM2_EMPTY_BASES							 __declspec(empty_bases)
#else
	#define FFSM2_EMPTY_BASES
#endif

//------------------------------------------------------------------------------

#define FFSM2_ARCHITECTURE(A)							FFSM2_ARCHITECTURE_##A()

#if INTPTR_MAX == INT64_MAX
	#define FFSM2_ARCHITECTURE_64()											true
	#define FFSM2_ARCHITECTURE_32()										   false

	#define FFSM2_64BIT_OR_32BIT(p64, p32)									 p64
#elif INTPTR_MAX == INT32_MAX
	#define FFSM2_ARCHITECTURE_64()										   false
	#define FFSM2_ARCHITECTURE_32()											true

	#define FFSM2_64BIT_OR_32BIT(p64, p32)									 p32
#else
	#error
#endif

//------------------------------------------------------------------------------

#if defined _DEBUG && defined _MSC_VER
	#define FFSM2_BREAK()										  __debugbreak()
	#define FFSM2_BREAK_AVAILABLE()											true
#else
	#define FFSM2_BREAK()											  ((void) 0)
	#define FFSM2_BREAK_AVAILABLE()										   false
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
	#define FFSM2_IF_DEBUG(...)										 __VA_ARGS__
	#define FFSM2_UNLESS_DEBUG(...)
	#define FFSM2_DEBUG_OR(y, n)											   y
#else
	#define FFSM2_IF_DEBUG(...)
	#define FFSM2_UNLESS_DEBUG(...)									 __VA_ARGS__
	#define FFSM2_DEBUG_OR(y, n)											   n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_BREAK_AVAILABLE() && defined FFSM2_ENABLE_ASSERT
	#define FFSM2_ASSERT_AVAILABLE()										true
	#define FFSM2_IF_ASSERT(...)									 __VA_ARGS__
	#define FFSM2_CHECKED(x)					   (!!(x) || (FFSM2_BREAK(), 0))
	#define FFSM2_ASSERT(x)						   (!!(x) || (FFSM2_BREAK(), 0))
	#define FFSM2_ASSERT_OR(y, n)											   y
#else
	#define FFSM2_ASSERT_AVAILABLE()									   false
	#define FFSM2_IF_ASSERT(...)
	#define FFSM2_CHECKED(x)												   x
	#define FFSM2_ASSERT(x)											  ((void) 0)
	#define FFSM2_ASSERT_OR(y, n)											   n
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_ALL
	#define FFSM2_ENABLE_DEBUG_STATE_TYPE
	#define FFSM2_ENABLE_PLANS
	#define FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_ENABLE_STRUCTURE_REPORT
	#define FFSM2_ENABLE_TRANSITION_HISTORY
#endif

//------------------------------------------------------------------------------

#ifndef FFSM2_DISABLE_TYPEINDEX
	#define FFSM2_TYPEINDEX_AVAILABLE()										true
	#define FFSM2_IF_TYPEINDEX(...)									 __VA_ARGS__
	#define FFSM2_TYPEINDEX_MASK										(1 << 0)
#else
	#define FFSM2_TYPEINDEX_AVAILABLE()									   false
	#define FFSM2_IF_TYPEINDEX(...)
	#define FFSM2_TYPEINDEX_MASK										(0 << 0)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_DEBUG_STATE_TYPE
	#define FFSM2_DEBUG_STATE_TYPE_AVAILABLE()								true
	#define FFSM2_DEBUG_STATE_TYPE_MASK									(1 << 1)
#else
	#define FFSM2_DEBUG_STATE_TYPE_AVAILABLE()							   false
	#define FFSM2_DEBUG_STATE_TYPE_MASK									(0 << 1)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_PLANS_AVAILABLE()											true
	#define FFSM2_IF_PLANS(...)										 __VA_ARGS__
	#define FFSM2_PLANS_MASK											(1 << 2)
#else
	#define FFSM2_PLANS_AVAILABLE()										   false
	#define FFSM2_IF_PLANS(...)
	#define FFSM2_PLANS_MASK											(0 << 2)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_SERIALIZATION_AVAILABLE()									true
	#define FFSM2_IF_SERIALIZATION(...)								 __VA_ARGS__
	#define FFSM2_SERIALIZATION_MASK									(1 << 3)
#else
	#define FFSM2_SERIALIZATION_AVAILABLE()								   false
	#define FFSM2_IF_SERIALIZATION(...)
	#define FFSM2_SERIALIZATION_MASK									(0 << 3)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_STRUCTURE_REPORT
	#define FFSM2_STRUCTURE_REPORT_AVAILABLE()								true
	#define FFSM2_IF_STRUCTURE_REPORT(...)							 __VA_ARGS__
	#define FFSM2_STRUCTURE_REPORT_MASK									(1 << 4)
#else
	#define FFSM2_STRUCTURE_REPORT_AVAILABLE()							   false
	#define FFSM2_IF_STRUCTURE_REPORT(...)
	#define FFSM2_STRUCTURE_REPORT_MASK									(0 << 4)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	#define FFSM2_TRANSITION_HISTORY_AVAILABLE()							true
	#define FFSM2_IF_TRANSITION_HISTORY(...)						 __VA_ARGS__
	#define FFSM2_TRANSITION_HISTORY_MASK								(1 << 5)
#else
	#define FFSM2_TRANSITION_HISTORY_AVAILABLE()						   false
	#define FFSM2_IF_TRANSITION_HISTORY(...)
	#define FFSM2_TRANSITION_HISTORY_MASK								(0 << 5)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_UTILITY_THEORY
	#define FFSM2_UTILITY_THEORY_AVAILABLE()								true
#else
	#define FFSM2_UTILITY_THEORY_AVAILABLE()							   false
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define FFSM2_ENABLE_LOG_INTERFACE

	#define FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()								true
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								(1 << 7)
#else
	#define FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()							   false
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								(0 << 7)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	#define FFSM2_LOG_INTERFACE_AVAILABLE()									true
	#define FFSM2_IF_LOG_INTERFACE(...)								 __VA_ARGS__
	#define FFSM2_LOG_INTERFACE_MASK									(1 << 8)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)				   \
		if (_logger)														   \
			_logger->recordTransition(CONTEXT, ORIGIN, DESTINATION)

#if FFSM2_PLANS_AVAILABLE()
	#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)					   \
		if (_logger)														   \
			_logger->recordTaskStatus(CONTEXT, ORIGIN, STATUS)

	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)							   \
		if (_logger)														   \
			_logger->recordPlanStatus(CONTEXT, STATUS)
#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)					   \
		if (_logger)														   \
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#else

	#define FFSM2_LOG_INTERFACE_AVAILABLE()								   false
	#define FFSM2_IF_LOG_INTERFACE(...)
	#define FFSM2_LOG_INTERFACE_MASK									(0 << 8)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)

#if FFSM2_PLANS_AVAILABLE()
	#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)
	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)
#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#endif

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._logger)							   \
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

#elif FFSM2_LOG_INTERFACE_AVAILABLE()

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._logger)							   \
			log(METHOD, *logger, control.context(), METHOD_ID)

#else

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

#endif

////////////////////////////////////////////////////////////////////////////////

namespace ffsm2 {

using FeatureTag = uint16_t;

constexpr FeatureTag FFSM2_FEATURE_TAG = FFSM2_TYPEINDEX_MASK
									   | FFSM2_DEBUG_STATE_TYPE_MASK
									   | FFSM2_PLANS_MASK
									   | FFSM2_SERIALIZATION_MASK
									   | FFSM2_STRUCTURE_REPORT_MASK
									   | FFSM2_TRANSITION_HISTORY_MASK
									   | FFSM2_VERBOSE_DEBUG_LOG_MASK
									   | FFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef FFSM2_TYPEINDEX_MASK
#undef FFSM2_DEBUG_STATE_TYPE_MASK
#undef FFSM2_PLANS_MASK
#undef FFSM2_SERIALIZATION_MASK
#undef FFSM2_STRUCTURE_REPORT_MASK
#undef FFSM2_TRANSITION_HISTORY_MASK
#undef FFSM2_VERBOSE_DEBUG_LOG_MASK
#undef FFSM2_LOG_INTERFACE_MASK

////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER == 1900
	#pragma warning(push)
	#pragma warning(disable: 4814) // in C++14 'constexpr' will not imply 'const'; consider explicitly specifying 'const'
#endif

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextra-semi" // error : extra ';' inside a class
	#pragma clang diagnostic ignored "-Wconstexpr-not-const" // error: 'constexpr' non-static member function will not be implicitly 'const' in C++14; add 'const' to avoid a change in behavior
#endif

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic" // error : extra ';'
#endif

//------------------------------------------------------------------------------

namespace ffsm2 {

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload final {};

struct Automatic;
struct Manual;

//------------------------------------------------------------------------------

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using Long		 = uint8_t;
static constexpr Long		INVALID_LONG		= UINT8_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

template <bool B,
		  typename TT,
		  typename TF>
struct ConditionalT final {
	using Type = TT;
};

template <typename TT,
		  typename TF>
struct ConditionalT<false, TT, TF> final {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <bool B,
		  typename TT,
		  typename TF>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct RemoveConstT final {
	using Type = T;
};

template <typename T>
struct RemoveConstT<const T> final {
	using Type = T;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using RemoveConst = typename RemoveConstT<T>::Type;

//------------------------------------------------------------------------------

template <typename T>
struct RemoveReferenceT final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&> final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&&> final {
	using Type = T;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using RemoveReference = typename RemoveReferenceT<T>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using Undecorate = RemoveConst<RemoveReference<T>>;

//------------------------------------------------------------------------------

template <typename T>
struct IsValueReferenceT final {
	static const bool VALUE = false;
};

template <typename T>
struct IsValueReferenceT<T&> final {
	static const bool VALUE = true;
};

////////////////////////////////////////////////////////////////////////////////

template <uint64_t N>
using UIndex = Conditional<N <= (1ull <<  8),  uint8_t,
			   Conditional<N <= (1ull << 16), uint16_t,
			   Conditional<N <= (1ull << 32), uint32_t,
												  void>>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <uint64_t N>
using UCapacity = Conditional<N < (1ull <<  8),  uint8_t,
				  Conditional<N < (1ull << 16), uint16_t,
				  Conditional<N < (1ull << 32), uint32_t,
													void>>>;

////////////////////////////////////////////////////////////////////////////////

FFSM2_CONSTEXPR(11)
uint32_t
bitWidth(const uint32_t v)							  noexcept {
	return	v		== 0 ?  0 :
			v >>  1 == 0 ?  1 :
			v >>  2 == 0 ?  2 :
			v >>  3 == 0 ?  3 :
			v >>  4 == 0 ?  4 :
			v >>  5 == 0 ?  5 :
			v >>  6 == 0 ?  6 :
			v >>  7 == 0 ?  7 :

			v >>  8 == 0 ?  8 :
			v >>  9 == 0 ?  9 :
			v >> 10 == 0 ? 10 :
			v >> 11 == 0 ? 11 :
			v >> 12 == 0 ? 12 :
			v >> 13 == 0 ? 13 :
			v >> 14 == 0 ? 14 :
			v >> 15 == 0 ? 15 :

			v >> 16 == 0 ? 16 :
			v >> 17 == 0 ? 17 :
			v >> 18 == 0 ? 18 :
			v >> 19 == 0 ? 19 :
			v >> 20 == 0 ? 20 :
			v >> 21 == 0 ? 21 :
			v >> 22 == 0 ? 22 :
			v >> 23 == 0 ? 23 :

			v >> 24 == 0 ? 24 :
			v >> 25 == 0 ? 25 :
			v >> 26 == 0 ? 26 :
			v >> 27 == 0 ? 27 :
			v >> 28 == 0 ? 28 :
			v >> 29 == 0 ? 29 :
			v >> 30 == 0 ? 30 :
			v >> 31 == 0 ? 31 :

						   32 ;
}

//------------------------------------------------------------------------------

template <uint64_t N>
using UBitWidth = Conditional<N <=  8,  uint8_t,
				  Conditional<N <= 16, uint16_t,
				  Conditional<N <= 32, uint32_t,
										   void>>>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>& t)						  noexcept	{
	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>&& t)						  noexcept	{
	static_assert(!IsValueReferenceT<T>::VALUE, "");

	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
FFSM2_CONSTEXPR(11)
RemoveReference<T>&&
move(T&& t)											  noexcept	{
	return static_cast<RemoveReference<T>&&>(t);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T0,
		  typename T1>
FFSM2_CONSTEXPR(11)
T0
min(const T0 t0,
	const T1 t1)									  noexcept
{
	return t0 < (T0) t1 ?
		   t0 : (T0) t1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T0,
		  typename T1>
FFSM2_CONSTEXPR(11)
T0
max(const T0 t0,
	const T1 t1)									  noexcept
{
	return t0 > (T0) t1 ?
		   t0 : (T0) t1;
}

//------------------------------------------------------------------------------

template <typename TIndex,
		  typename TElement,
		  TIndex NCount>
FFSM2_CONSTEXPR(11)
TIndex
count(const TElement(&)[NCount])					  noexcept	{
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T,
		  typename TT>
FFSM2_CONSTEXPR(11)
T
contain(const T x,
		const TT to)								  noexcept	{ return (x + (T) to - 1) / (T) to;		}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_CONSTEXPR(14)
void
fill(T& a, const char value)						  noexcept { memset(&a, (int) value, sizeof(a));	}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConstT;

template <typename>
struct StaticPrintTypeT;

////////////////////////////////////////////////////////////////////////////////

}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	FFSM2_CONSTEXPR(11)	IteratorT(Container& container,
								  const Index cursor)									  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& FFSM2_IF_ASSERT(other))	const noexcept	{
		FFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	FFSM2_CONSTEXPR(14)	IteratorT& operator ++()										  noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	FFSM2_CONSTEXPR(14)		  Item& operator *()		  noexcept	{ return  _container[_cursor];	}
	FFSM2_CONSTEXPR(11)	const Item& operator *()	const noexcept	{ return  _container[_cursor];	}

	FFSM2_CONSTEXPR(14)		  Item* operator->()		  noexcept	{ return &_container[_cursor];	}
	FFSM2_CONSTEXPR(11)	const Item* operator->()	const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Index _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	FFSM2_CONSTEXPR(11)	IteratorT(const Container& container,
								  const Index cursor)									  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& FFSM2_IF_ASSERT(other))	const noexcept	{
		FFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	FFSM2_CONSTEXPR(14)	IteratorT& operator ++()										  noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	FFSM2_CONSTEXPR(11)	const Item& operator *()	const noexcept	{ return _container[_cursor];	}

	FFSM2_CONSTEXPR(11)	const Item* operator->()	const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}
#if FFSM2_SERIALIZATION_AVAILABLE()

namespace ffsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <Long>
class BitWriteStreamT;

template <Long>
class BitReadStreamT;

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class StreamBufferT {
	template <Long>
	friend class BitWriteStreamT;

	template <Long>
	friend class BitReadStreamT;

public:
	static constexpr Long BIT_CAPACITY	= NBitCapacity;
	static constexpr Long BYTE_COUNT	= contain(BIT_CAPACITY, 8u);

	using Data = uint8_t[BYTE_COUNT];

	FFSM2_CONSTEXPR(14)	void clear()										  noexcept	{ fill(_data, 0);	}

	FFSM2_CONSTEXPR(14)		  Data& data()									  noexcept	{ return _data;		}
	FFSM2_CONSTEXPR(11)	const Data& data()								const noexcept	{ return _data;		}

private:
	Data _data;
};

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class BitWriteStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	FFSM2_CONSTEXPR(14)	explicit BitWriteStreamT(Buffer& buffer,
												 const Long cursor = 0)		  noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{
		_buffer.clear();
	}

	template <Short NBitWidth>
	FFSM2_CONSTEXPR(14)	void write(const UBitWidth<NBitWidth> item)			  noexcept;

	FFSM2_CONSTEXPR(11)	Long cursor()									const noexcept	{ return _cursor;	}

private:
	Buffer& _buffer;

	Long _cursor = 0;
};

//------------------------------------------------------------------------------

template <Long NBitCapacity>
class BitReadStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	FFSM2_CONSTEXPR(11)	explicit BitReadStreamT(const Buffer& buffer,
												const Long cursor = 0)		  noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{}

	template <Short NBitWidth>
	FFSM2_CONSTEXPR(14)	UBitWidth<NBitWidth> read()							  noexcept;

	FFSM2_CONSTEXPR(11)	Long cursor()									const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#if FFSM2_SERIALIZATION_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
template <Short NBitWidth>
FFSM2_CONSTEXPR(14)
void
BitWriteStreamT<NBC>::write(const UBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
template <Short NBitWidth>
FFSM2_CONSTEXPR(14)
UBitWidth<NBitWidth>
BitReadStreamT<NBC>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item item = 0;
	Short itemCursor = 0;

	for (Short itemWidth = BIT_WIDTH; itemWidth; )
		if (FFSM2_CHECKED(_cursor + itemWidth <= BIT_CAPACITY)) {
			const Long	byteIndex		= _cursor >> 3;
			const uint8_t& byte			= _buffer._data[byteIndex];

			const Short byteChunkStart	= _cursor & 0x7;
			const Short byteDataWidth	= 8 - byteChunkStart;
			const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
			const Short byteChunkMask	= (1 << byteChunkWidth) - 1;
			const Item	byteChunk		= (byte >> byteChunkStart) & byteChunkMask;
			const Item	itemChunk		= byteChunk << itemCursor;

			item		|= itemChunk;
			itemCursor	+= byteChunkWidth;
			itemWidth	-= byteChunkWidth;
			_cursor		+= byteChunkWidth;
		}

	return item;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// SPECIFIC

template <typename>
struct TypeT {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TL_ final
	: TypeT<Ts>...
{
	static constexpr Long SIZE = sizeof...(Ts);
};

// SPECIFIC
//------------------------------------------------------------------------------

template <Long N>
struct Const {
	static constexpr Long VALUE = N;
};

//------------------------------------------------------------------------------

template <typename, typename>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TL_<Ts...>> final {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using PrependTypes = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <typename, typename>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TL_<Ts1...>, TL_<Ts2...>> final {
	using Type = TL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct LowerT;

template <Long NHalf, Long NIndex, typename... Ts>
using LowerTypes = typename LowerT<NHalf, NIndex, Ts...>::Type;

template <Long NHalf, Long NIndex, typename TFirst, typename... TRest>
struct LowerT<NHalf, NIndex, TFirst, TRest...> final {
	using LTypeList = typename LowerT<NHalf, NIndex + 1, TRest...>::Type;

	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependTypes<TFirst, LTypeList>,
					 LTypeList
				 >;
};

template <Long NHalf, Long NIndex>
struct LowerT<NHalf, NIndex> final {
	using Type = TL_<>;
};

template <typename... Ts>
using LHalfTypes = LowerTypes<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct UpperT;

template <Long NHalf, Long NIndex, typename... Ts>
using UpperTypes = typename UpperT<NHalf, NIndex, Ts...>::Type;

template <Long NHalf, Long NIndex, typename TFirst, typename... TRest>
struct UpperT<NHalf, NIndex, TFirst, TRest...> final {
	using Type = Conditional<
					 (NIndex < NHalf),
					 UpperTypes<NHalf, NIndex + 1, TRest...>,
					 TL_<TFirst, TRest...>
				 >;
};

template <Long NHalf, Long NIndex>
struct UpperT<NHalf, NIndex> final {
	using Type = TL_<>;
};

template <typename... Ts>
using RHalfTypes = UpperTypes<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

template<Long, typename...>
struct FindImpl
	: Const<INVALID_LONG>
{};

template<Long N, typename T, typename TFirst, typename... TRest>
struct FindImpl<N, T, TFirst, TRest...>
	: FindImpl<N + 1, T, TRest...>
{};

template<Long N, typename T, typename... Ts>
struct FindImpl<N, T, T, Ts...>
	: Const<N>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename>
struct Find;

template <typename T, typename... Ts>
struct Find<TL_<Ts...>, T> final
	: FindImpl<0, T, Ts...>
{};

////////////////////////////////////////////////////////////////////////////////
// SPECIFIC

}

template <typename TList, typename T>
constexpr Long index   () noexcept { return detail::Find<TList, T>::VALUE;						}

template <typename TList, typename T>
constexpr bool contains() noexcept { return std::is_base_of<detail::TypeT<T>, TList>::value;	}

// SPECIFIC
//------------------------------------------------------------------------------

}

namespace ffsm2 {

////////////////////////////////////////////////////////////////////////////////

enum class Method : uint8_t {
	NONE,

	ENTRY_GUARD,
	ENTER,
	REENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,

#if FFSM2_PLANS_AVAILABLE()
	PLAN_SUCCEEDED,
	PLAN_FAILED,
#endif

	COUNT
};

#if FFSM2_PLANS_AVAILABLE()

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

#if FFSM2_TYPEINDEX_AVAILABLE()

static
inline
const char*
stateName(const std::type_index stateType)							  noexcept {
	const char* const raw = stateType.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;
		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
}

#endif

//------------------------------------------------------------------------------

static
FFSM2_CONSTEXPR(14)
const char*
methodName(const Method method)										  noexcept {
	switch (method) {
	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::REENTER:		 return "reenter";
	case Method::UPDATE:		 return "update";
	case Method::REACT:			 return "react";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";

#if FFSM2_PLANS_AVAILABLE()
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
#endif

	default:
		FFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct alignas(4) TransitionBase {
	FFSM2_CONSTEXPR(11)
	TransitionBase() noexcept = default;

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)
	TransitionBase(const StateID destination_)						  noexcept
		: destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	TransitionBase(const StateID origin_,
				   const StateID destination_)						  noexcept
		: origin	 {origin_}
		, destination{destination_}
	{}

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionBase& other)					const noexcept {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionBase& other)					const noexcept {
		return origin	   != other.origin ||
			   destination != other.destination ||
			   method	   != other.method;
	}

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)
	explicit
	operator bool()												const noexcept {
		return destination != INVALID_STATE_ID;
	}

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(14)
	void
	clear()															  noexcept {
		destination	= INVALID_STATE_ID;
	}

	//----------------------------------------------------------------------

	StateID	origin		= INVALID_STATE_ID;
	StateID	destination	= INVALID_STATE_ID;
	Method	method		= Method::NONE;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload>
struct alignas(4) TransitionT final
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 4>::type;

	//----------------------------------------------------------------------

	using TransitionBase::TransitionBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT()													  noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination,
				const Payload& payload)								  noexcept
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination,
				Payload&& payload)									  noexcept
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{move(payload)};
	}

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin,
				const StateID destination,
				const Payload& payload)								  noexcept
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin,
				const StateID destination,
				Payload&& payload)									  noexcept
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{move(payload)};
	}

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionT& other)						const noexcept {
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload == other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionT& other)						const noexcept {
		return TransitionBase::operator != (other) ||
			   (payloadSet != other.payloadSet);
		//	   (payloadSet |= other.payloadSet || payload != other.payload);
	}

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)
	const Payload*
	payload()													const noexcept {
		return payloadSet ?
			reinterpret_cast<const Payload*>(&storage) : nullptr;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	bool payloadSet = false;
	Storage storage;
};

//------------------------------------------------------------------------------

template <>
struct alignas(4) TransitionT<void> final
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}

struct Request final {
	Short index;
};

////////////////////////////////////////////////////////////////////////////////

}
namespace ffsm2 {

#if FFSM2_LOG_INTERFACE_AVAILABLE()

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
		, typename TContext = EmptyContext>
struct LoggerInterfaceT {
	using Context		 = TContext;

	using Method		 = ::ffsm2::Method;
	using StateID		 = ::ffsm2::StateID;

#if FFSM2_PLANS_AVAILABLE()
	using StatusEvent	 = ::ffsm2::StatusEvent;
#endif

	FFSM2_CONSTEXPR(NO)
	virtual
	void recordMethod(Context& FFSM2_UNUSED(context),
					  const StateID FFSM2_UNUSED(origin),
					  const Method FFSM2_UNUSED(method))			  noexcept
	{}

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTransition(Context& FFSM2_UNUSED(context),
					 const StateID FFSM2_UNUSED(origin),
					 const StateID FFSM2_UNUSED(target))			  noexcept
	{}

#if FFSM2_PLANS_AVAILABLE()

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTaskStatus(Context& FFSM2_UNUSED(context),
					 const StateID FFSM2_UNUSED(origin),
					 const StatusEvent FFSM2_UNUSED(event))			  noexcept
	{}

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordPlanStatus(Context& FFSM2_UNUSED(context),
					 const StatusEvent FFSM2_UNUSED(event))			  noexcept
	{}

#endif

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordCancelledPending(Context& FFSM2_UNUSED(context),
						   const StateID FFSM2_UNUSED(origin))		  noexcept
	{}

};

////////////////////////////////////////////////////////////////////////////////

#else

template <FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
		, typename TContext = EmptyContext>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class StaticArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      StaticArrayT>;
	using CIterator	= IteratorT<const StaticArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	FFSM2_CONSTEXPR(14)	StaticArrayT() = default;
	FFSM2_CONSTEXPR(14)	StaticArrayT(const Item filler)						  noexcept	{ fill(filler);						}

	template <typename N>
	FFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				  noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)			const noexcept;

	FFSM2_CONSTEXPR(11)	Index count()									const noexcept	{ return CAPACITY;					}

	FFSM2_CONSTEXPR(14)	void fill(const Item filler)						  noexcept;
	FFSM2_CONSTEXPR(14)	void clear()										  noexcept	{ fill(INVALID_SHORT);				}

	FFSM2_CONSTEXPR(14)	 Iterator  begin()									  noexcept	{ return  Iterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator  begin()								const noexcept	{ return CIterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator cbegin()								const noexcept	{ return CIterator(*this, first());	}

	FFSM2_CONSTEXPR(14)	 Iterator	 end()									  noexcept	{ return  Iterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	 end()								const noexcept	{ return CIterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	cend()								const noexcept	{ return CIterator(*this, limit());	}

private:
	FFSM2_CONSTEXPR(11)	Index first()									const noexcept	{ return 0;							}
	FFSM2_CONSTEXPR(11)	Index  next(const Index index)					const noexcept	{ return index + 1;					}
	FFSM2_CONSTEXPR(11)	Index limit()									const noexcept	{ return CAPACITY;					}

private:
	Item _items[CAPACITY] {};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArrayT<T, 0> final {
	using Item		= T;

	FFSM2_CONSTEXPR(11)	StaticArrayT() = default;
	FFSM2_CONSTEXPR(11)	StaticArrayT(const Item) noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class ArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      ArrayT>;
	using CIterator	= IteratorT<const ArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	FFSM2_CONSTEXPR(14)	 void clear()										  noexcept	{ _count = 0;						}

	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(const TArgs &... args)				  noexcept;

	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(		TArgs&&... args)				  noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				  noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)			const noexcept;

	FFSM2_CONSTEXPR(11)	Index  count()									const noexcept	{ return _count;					}

	FFSM2_CONSTEXPR(14)	ArrayT& operator += (const Item & item)				  noexcept;
	FFSM2_CONSTEXPR(14)	ArrayT& operator += (	   Item&& item)				  noexcept;

	template <Long N>
	FFSM2_CONSTEXPR(14)	ArrayT& operator += (const ArrayT<Item, N>& other)	  noexcept;

	FFSM2_CONSTEXPR(14)	 Iterator  begin()									  noexcept	{ return  Iterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator  begin()								const noexcept	{ return CIterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator cbegin()								const noexcept	{ return CIterator(*this, first());	}

	FFSM2_CONSTEXPR(14)	 Iterator	 end()									  noexcept	{ return  Iterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	 end()								const noexcept	{ return CIterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	cend()								const noexcept	{ return CIterator(*this, limit());	}

private:
	FFSM2_CONSTEXPR(11)	Index first()									const noexcept	{ return 0;							}
	FFSM2_CONSTEXPR(11)	Index next(const Index index)					const noexcept	{ return index + 1;					}
	FFSM2_CONSTEXPR(11)	Index limit()									const noexcept	{ return _count;					}

private:
	Index _count = 0;
	Item _items[CAPACITY] {};
};

//------------------------------------------------------------------------------

template <typename T>
class ArrayT<T, 0> final {
public:
	using Item	= T;
	using Index	= UCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC>::operator[] (const N index) noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
const T&
StaticArrayT<T, NC>::operator[] (const N index) const noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(const TArgs&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{args...};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(TArgs&&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{forward<TArgs>(args)...};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) noexcept {
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
const typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) const noexcept {
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

//------------------------------------------------------------------------------
// SPECIFIC

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const Item& item) noexcept {
	emplace(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (Item&& item) noexcept {
	emplace(move(item));

	return *this;
}

// SPECIFIC
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
FFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		emplace(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
class BitArrayT final {
public:
	using Index	= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, 8);

public:
	FFSM2_CONSTEXPR(14)	BitArrayT()								  noexcept	{ clear();	}

	FFSM2_CONSTEXPR(14)	void clear()							  noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	bool get  (const TIndex index)		const noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	void set  (const TIndex index)			  noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	void clear(const TIndex index)			  noexcept;

private:
	uint8_t _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <>
class BitArrayT<0> final {
public:
	FFSM2_CONSTEXPR(14)	void clear()							  noexcept	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// COMMON

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get(const TIndex index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = (Index) index / 8;
	const Index bit  = (Index) index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = (Index) index / 8;
	const Index bit  = (Index) index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = (Index) index / 8;
	const Index bit  = (Index) index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskBase {
	FFSM2_CONSTEXPR(11)	TaskBase()								  noexcept {}

	FFSM2_CONSTEXPR(11)	TaskBase(const StateID origin_,
								 const StateID destination_)	  noexcept
		: origin{origin_}
		, destination{destination_}
	{}

	static_assert(sizeof(Long) == sizeof(StateID), "");

	union {
		StateID origin		= INVALID_STATE_ID;
		Long prev;
	};

	union {
		StateID destination	= INVALID_STATE_ID;
		Long next;
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FFSM2_CONSTEXPR(11)
bool
operator == (const TaskBase& lhs,
			 const TaskBase& rhs)								  noexcept
{
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination;
}

//------------------------------------------------------------------------------

template <typename TPayload>
struct TaskT final
	: TaskBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 2>::type;

	using TaskBase::TaskBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	TaskT()									  noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	TaskT(const StateID origin,
							  const StateID destination,
							  const Payload& payload)			  noexcept
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	TaskT(const StateID origin,
							  const StateID destination,
							  Payload&& payload)				  noexcept
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{move(payload)};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	Storage storage;
	bool payloadSet = false;
};

//------------------------------------------------------------------------------

template <>
struct TaskT<void> final
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload, Long NCapacity>
class TaskListT {
public:
	using Index		= Long;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index INVALID	= Index (-1);

private:
	using Payload	= TPayload;
	using Item		= TaskT<Payload>;

public:
	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(TArgs&&... args)							  noexcept;

	FFSM2_CONSTEXPR(14)	void remove(const Index i)								  noexcept;

	FFSM2_CONSTEXPR(14)		  Item& operator[] (const Index i)					  noexcept;
	FFSM2_CONSTEXPR(11)	const Item& operator[] (const Index i)				const noexcept;

	FFSM2_CONSTEXPR(11)	Index count()										const noexcept	{ return _count;	}

private:
	FFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)	const noexcept);

private:
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last  = 0;
	Index _count = 0;
	Item _items[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TItem>
class TaskListT<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TP, Long NC>
template <typename... TA>
FFSM2_CONSTEXPR(14)
Long
TaskListT<TP, NC>::emplace(TA&&... args) noexcept {
	FFSM2_ASSERT(_last  <= CAPACITY);

	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		Item& item = _items[index];

		if (_vacantHead != _vacantTail) {
			// recycle
			FFSM2_ASSERT(item.prev == INVALID);
			FFSM2_ASSERT(item.next != INVALID);

			_vacantHead = item.next;

			Item& head = _items[_vacantHead];
			FFSM2_ASSERT(head.prev == index);
			head.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			Item& vacant = _items[_vacantHead];
			vacant.prev = INVALID;
			vacant.next = INVALID;
		} else {
			// last
			FFSM2_ASSERT(_count == CAPACITY - 1);

			_last = CAPACITY;
			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		new (&item) Item{forward<TA>(args)...};
		++_count;

		FFSM2_IF_ASSERT(verifyStructure());

		return index;
	} else {
		// full
		FFSM2_ASSERT(_vacantHead == INVALID);
		FFSM2_ASSERT(_vacantTail == INVALID);
		FFSM2_ASSERT(_count		 == CAPACITY);
		FFSM2_BREAK();

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
FFSM2_CONSTEXPR(14)
void
TaskListT<TP, NC>::remove(const Index i) noexcept {
	FFSM2_ASSERT(i < CAPACITY && _count);

	Item& item = _items[i];

	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		item.prev = INVALID;
		item.next = _vacantHead;

		Item& head = _items[_vacantHead];
		head.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		FFSM2_ASSERT(_count		 == CAPACITY);
		FFSM2_ASSERT(_vacantHead == INVALID);
		FFSM2_ASSERT(_vacantTail == INVALID);

		item.prev = INVALID;
		item.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	FFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
FFSM2_CONSTEXPR(14)
typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TP, Long NC>
FFSM2_CONSTEXPR(11)
const typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) const noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

template <typename TP, Long NC>
void
TaskListT<TP, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		FFSM2_ASSERT(_items[_vacantHead].prev == INVALID);
		FFSM2_ASSERT(_items[_vacantTail].next == INVALID);

		Index emptyCount = 1;

		for (Index c = _vacantHead; c != _vacantTail; ) {
			FFSM2_ASSERT(occupied != c);

			const Item& current = _items[c];

			const Long f = current.next;
			if (f != INVALID) {
				// next
				const Item& following = _items[f];

				FFSM2_ASSERT(following.prev == c);

				c = f;
				continue;
			} else {
				// end
				FFSM2_ASSERT(_vacantTail == c);
				FFSM2_ASSERT(_count		 == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		FFSM2_ASSERT(_vacantHead == INVALID);
		FFSM2_ASSERT(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename
		, typename
		, typename
		FFSM2_IF_SERIALIZATION(, Long)
		, Long
		, Long
		, typename>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>> final
{
	using StateList		= TStateList;
	using Payload		= TPayload;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<Payload>;
	using Tasks			= TaskListT<Payload, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads		= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBits		= BitArrayT<StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus(const StateID stateId)			  noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId)	const noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()								const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>> final
{
	using StateList		= TStateList;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= TaskListT<void, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBits		= BitArrayT<StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus(const StateID stateId)			  noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId)	const noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()								const noexcept;
#endif
};

//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

}
}

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlan() const noexcept	{
	Long length = 0;
	const Bounds& bounds = tasksBounds;

	if (bounds.first != INVALID_LONG) {
		FFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				FFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					FFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				FFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		FFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlan() const noexcept	{
	Long length = 0;
	const Bounds& bounds = tasksBounds;

	if (bounds.first != INVALID_LONG) {
		FFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				FFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					FFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				FFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		FFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status final {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	FFSM2_CONSTEXPR(11)	Status(const Result result_ = Result::NONE)	  noexcept
		: result{result_}
	{}

	FFSM2_CONSTEXPR(11)	explicit operator bool()				const noexcept	{ return result != Result::NONE;	}

	FFSM2_CONSTEXPR(14)	void clear()								  noexcept	{ result = Result::NONE;			}
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FFSM2_CONSTEXPR(14)
Status
combine(const Status lhs, const Status rhs)							  noexcept	{
	const Status::Result result = lhs.result > rhs.result ?
									  lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TArgs>
class CPlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	//----------------------------------------------------------------------

	struct IteratorT final {
		FFSM2_CONSTEXPR(14)	IteratorT(const CPlanT& plan)		  noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()		const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()					  noexcept;

		FFSM2_CONSTEXPR(11)	const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()						const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	//----------------------------------------------------------------------

private:
	FFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData)		  noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds}
	{}

	template <typename T>
	static constexpr StateID  stateId()							  noexcept	{ return			index<StateList , T>();	}

public:
	FFSM2_CONSTEXPR(14)	explicit operator bool()			const noexcept;

	FFSM2_CONSTEXPR(14)	IteratorT first()						  noexcept	{ return IteratorT{*this};					}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanBaseT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct IteratorT final {
		FFSM2_CONSTEXPR(14)	IteratorT(PlanBaseT& plan)			  noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()		const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()					  noexcept;

		FFSM2_CONSTEXPR(14)	Task& operator  *()					  noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(14)	Task* operator ->()					  noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	void remove()						  noexcept	{ _plan.remove(_curr);						}

		FFSM2_CONSTEXPR(14)	Long next()						const noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator final {
		FFSM2_CONSTEXPR(14)	CIterator(const PlanBaseT& plan)	  noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()		const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()					  noexcept;

		FFSM2_CONSTEXPR(14)	const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()						const noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_CONSTEXPR(11)	PlanBaseT(PlanData& planData)			  noexcept;

	template <typename T>
	static constexpr StateID  stateId()							  noexcept	{ return		index<StateList , T>();		}

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination)	  noexcept;

	FFSM2_CONSTEXPR(14)	bool linkTask(const Long index)			  noexcept;

public:
	FFSM2_CONSTEXPR(14)	explicit operator bool()			const noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_CONSTEXPR(14)	void clear()							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool change(const StateID origin, const StateID destination)  noexcept	{ return append(origin			  , destination)			;	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool change(const StateID destination)						  noexcept	{ return change(stateId<TOrigin>(), destination)			;	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool change()												  noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return IteratorT to the first task
	FFSM2_CONSTEXPR(14)	 IteratorT first()											  noexcept	{ return  IteratorT{*this};										}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_CONSTEXPR(11)	CIterator first()										const noexcept	{ return CIterator{*this};										}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_CONSTEXPR(14)	void remove(const Long task)								  noexcept;

protected:
	PlanData& _planData;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				FFSM2_IF_SERIALIZATION(, NSerialBits)
				, NSubstitutionLimit
				, NTaskCapacity
				, TPayload>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   FFSM2_IF_SERIALIZATION(, NSerialBits)
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , TPayload>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									const Payload& payload)									  noexcept;

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									Payload&& payload)										  noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool changeWith(const StateID origin, const StateID destination, const Payload & payload) noexcept	{ return append(origin								 , destination								 ,		payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool changeWith(const StateID origin, const StateID destination,	   Payload&& payload) noexcept	{ return append(origin								 , destination								 , move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool changeWith(					  const StateID destination, const Payload & payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 ,		payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool changeWith(					  const StateID destination,	   Payload&& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool changeWith(												 const Payload & payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(),		payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool changeWith(													   Payload&& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				FFSM2_IF_SERIALIZATION(, NSerialBits)
				, NSubstitutionLimit
				, NTaskCapacity
				, void>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   FFSM2_IF_SERIALIZATION(, NSerialBits)
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , void>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>;

	using PlanBase = PlanBaseT<Args>;

	using PlanBase::PlanBase;
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TArgs>
FFSM2_CONSTEXPR(14)
CPlanT<TArgs>::IteratorT::IteratorT(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
CPlanT<TArgs>::IteratorT::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
CPlanT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
CPlanT<TArgs>::IteratorT::next() const noexcept {
	if (_curr < CPlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
CPlanT<TArgs>::operator bool() const noexcept {
	FFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::IteratorT::IteratorT(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::IteratorT::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
PlanBaseT<TArgs>::IteratorT::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::CIterator::CIterator(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::CIterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
PlanBaseT<TArgs>::CIterator::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(11)
PlanBaseT<TArgs>::PlanBaseT(PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::operator bool() const noexcept {
	FFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
bool
PlanBaseT<TArgs>::append(const StateID origin,
						 const StateID destination) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists = true;

		return linkTask(_planData.tasks.emplace(origin, destination));
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
bool
PlanBaseT<TArgs>::linkTask(const Long index) noexcept	{
	if (index != Tasks::INVALID) {
		if (_bounds.first == INVALID_LONG) {
			FFSM2_ASSERT(_bounds.last == INVALID_LONG);
			FFSM2_ASSERT(_planData.taskLinks[index].prev == INVALID_LONG);
			FFSM2_ASSERT(_planData.taskLinks[index].next == INVALID_LONG);

			_bounds.first = index;
			_bounds.last  = index;
		} else {
			FFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
			FFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

			auto& lastLink = _planData.taskLinks[_bounds.last];
			FFSM2_ASSERT(lastLink.next == INVALID_LONG);

			lastLink.next  = index;

			auto& currLink = _planData.taskLinks[index];
			FFSM2_ASSERT(currLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::clear() noexcept	{
	if (_bounds.first < TaskLinks::CAPACITY) {
		FFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			FFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const auto& link = _planData.taskLinks[index];
			FFSM2_ASSERT(index == _bounds.first ?
							 link.prev == INVALID_LONG :
							 link.prev <  TaskLinks::CAPACITY);

			const Long next = link.next;

			remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG;
		_bounds.last  = INVALID_LONG;
	} else {
		FFSM2_ASSERT(_bounds.first == INVALID_LONG);
		FFSM2_ASSERT(_bounds.last  == INVALID_LONG);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::remove(const Long index) noexcept {
	FFSM2_ASSERT(_planData.planExists);
	FFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
	FFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

	FFSM2_ASSERT(index < TaskLinks::CAPACITY);

	TaskLink& link = _planData.taskLinks[index];

	if (link.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[link.prev];
		prev.next = link.next;
	} else {
		FFSM2_ASSERT(_bounds.first == index);
		_bounds.first = link.next;
	}

	if (link.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[link.next];
		next.prev = link.prev;
	} else {
		FFSM2_ASSERT(_bounds.last == index);
		_bounds.last = link.prev;
	}

	link.prev = INVALID_LONG;
	link.next = INVALID_LONG;

	_planData.tasks.remove(index);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
bool
PlanT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																			   const StateID destination,
																			   const Payload& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, payload));
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
bool
PlanT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																			   const StateID destination,
																			   Payload&& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, move(payload)));
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename
		, typename
		, typename
		FFSM2_IF_SERIALIZATION(, Long)
		, Long
		FFSM2_IF_PLANS(, Long)
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry final {
	FFSM2_CONSTEXPR(11)	bool isActive()					const noexcept	{ return active != INVALID_SHORT;	}

	FFSM2_CONSTEXPR(14)	void clearRequests()				  noexcept	{ requested = INVALID_SHORT;		}

	Short active	= INVALID_SHORT;
	Short requested	= INVALID_SHORT;
};

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
	using CPlan				= CPlanT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		FFSM2_CONSTEXPR(14)	Origin(ControlT& control_,
								   const StateID stateId)				  noexcept;

		FFSM2_CONSTEXPR(20)	~Origin()									  noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	ControlT(Context& context
							   , Registry& registry
							   , Transition& request
							   FFSM2_IF_PLANS(, PlanData& planData)
							   FFSM2_IF_TRANSITION_HISTORY(, const Transition& previousTransition)
							   FFSM2_IF_LOG_INTERFACE(, Logger* const logger))		  noexcept
		: _context{context}
		, _registry{registry}
		, _request{request}
		FFSM2_IF_PLANS(, _planData{planData})
		FFSM2_IF_TRANSITION_HISTORY(, _previousTransition{previousTransition})
		FFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									  noexcept	{ return index<StateList , TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(14)		  Context& _()								  noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(11)	const Context& _()							const noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(14)		  Context& context()						  noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(11)	const Context& context()					const noexcept	{ return _context;						}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11)	const Transition& request()					const noexcept	{ return _request;						}

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	FFSM2_CONSTEXPR(11)	CPlan plan()								const noexcept	{ return CPlan{_planData};				}

#endif

	//----------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransition()'
	/// @return Array of last transition requests
	FFSM2_CONSTEXPR(11)	const Transition& previousTransitions()		const noexcept	{ return _previousTransition;			}

#endif

	//----------------------------------------------------------------------

protected:
	Context& _context;
	Registry& _registry;
	Transition& _request;
	StateID _originId = INVALID_STATE_ID;
	FFSM2_IF_PLANS(PlanData& _planData);
	FFSM2_IF_TRANSITION_HISTORY(const Transition& _previousTransition);
	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

//------------------------------------------------------------------------------

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	template <typename, typename>
	friend class RV_;

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;
#endif

	using Control::Control;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()
// COMMON

	/// @brief Access plan
	/// @return Plan
	FFSM2_CONSTEXPR(14)	  Plan plan()							  noexcept	{ return  Plan{_planData};						}

// COMMON
// COMMON

	/// @brief Access read-only plan
	/// @return Read-only plan
	FFSM2_CONSTEXPR(11)	CPlan plan()								const noexcept	{ return CPlan{_planData};				}

// COMMON
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_IF_PLANS(using Control::_planData);
	FFSM2_IF_LOG_INTERFACE(using Control::_logger);

	Status _status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlBaseT
	: public PlanControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using PlanControl	= PlanControlT<TArgs>;

	using typename PlanControl::StateList;
	using typename PlanControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using TasksBits		= BitArrayT<StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock final {
		FFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)	  noexcept;
		FFSM2_CONSTEXPR(20)	~Lock()								  noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId State identifier
	FFSM2_CONSTEXPR(14)	void changeTo(const StateID stateId)	  noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo()							  noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	//----------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	//----------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	FFSM2_CONSTEXPR(14)	void succeed()							  noexcept;

	/// @brief Fail a plan task for the current state
	FFSM2_CONSTEXPR(14)	void fail()								  noexcept;

#endif

	//----------------------------------------------------------------------

protected:
	using PlanControl::_request;
	FFSM2_IF_PLANS(using PlanControl::_planData);
	FFSM2_IF_LOG_INTERFACE(using PlanControl::_logger);

	using PlanControl::_originId;
	using PlanControl::_status;

	bool _locked = false;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
class FullControlT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   FFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  FFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  FFSM2_IF_PLANS(, NTaskCapacity)
								  , TPayload>>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 FFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#if FFSM2_PLANS_AVAILABLE()

	template <typename TState>
	FFSM2_CONSTEXPR(14)	void updatePlan(TState& headState,
										const Status subStatus)		  noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)		  noexcept	{ changeWith(FullControlBase::template stateId<TState>(),			payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(Payload&& payload)			  noexcept	{ changeWith(FullControlBase::template stateId<TState>(), move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//------------------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_request;
	FFSM2_IF_PLANS(using FullControlBase::_planData);
	FFSM2_IF_PLANS(using FullControlBase::_registry);
	FFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   FFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  FFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  FFSM2_IF_PLANS(, NTaskCapacity)
								  , void>>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 FFSM2_IF_PLANS(, NTaskCapacity)
					 , void>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#if FFSM2_PLANS_AVAILABLE()

	template <typename TState>
	FFSM2_CONSTEXPR(14)	void updatePlan(TState& headState,
										const Status subStatus)	  noexcept;

#endif

public:
	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	FFSM2_IF_PLANS(using FullControlBase::_registry);
	FFSM2_IF_PLANS(using FullControlBase::_planData);

	using FullControlBase::_status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Context;

	using typename FullControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	GuardControlT(Context& context
									, Registry& registry
									, Transition& request
									, const Transition& currentTransition
									, const Transition& pendingTransition
									FFSM2_IF_PLANS(, PlanData& planData)
									FFSM2_IF_TRANSITION_HISTORY(, const Transition& previousTransition)
									FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, request
					FFSM2_IF_PLANS(, planData)
					FFSM2_IF_TRANSITION_HISTORY(, previousTransition)
					FFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransition{currentTransition}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get current transition request
	/// @return Current transition request
	FFSM2_CONSTEXPR(11)	const Transition& currentTransitions()		const noexcept	{ return _currentTransition;			}

	/// @brief Get pending transition request
	/// @return Pending transition request
	FFSM2_CONSTEXPR(11)	const Transition& pendingTransition()		const noexcept	{ return _pendingTransition;			}

	/// @brief Cancel pending transition request
	///		(can be used to substitute a transition into the current state with a different one)
	FFSM2_CONSTEXPR(14)	void cancelPendingTransition()				  noexcept;

private:
	using FullControl::_registry;
	using FullControl::_originId;

	FFSM2_IF_LOG_INTERFACE(using FullControl::_logger);

	const Transition& _currentTransition;
	const Transition& _pendingTransition;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId) noexcept
	: control{control_}
	, prevId{control._originId}
{
	control._originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

// COMMON
////////////////////////////////////////////////////////////////////////////////
// COMMON

template <typename TArgs>
FFSM2_CONSTEXPR(14)
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
FullControlBaseT<TArgs>::Lock::~Lock() noexcept	{
	if (control)
		control->_locked = false;
}

// COMMON
//------------------------------------------------------------------------------
// COMMON

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// COMMON
//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////
#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::updatePlan(TState& headState,
																						  const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it)
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					it.remove();

					break;
				}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}
	}
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId,
																										   const Payload& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId,
																										   Payload&& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::updatePlan(TState& headState,
																						   const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it)
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					it.remove();

					break;
				}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
GuardControlT<TArgs>::cancelPendingTransition() noexcept {
	_cancelled = true;

	FFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
	template <typename...>
	friend struct B_;

protected:
	using Context		= typename TArgs::Context;

	using StateList		= typename TArgs::StateList;

	using Control		= ControlT	  <TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	FFSM2_CONSTEXPR(14)	void preEntryGuard	  (Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void preEnter		  (Context&)	  noexcept	{}
	FFSM2_CONSTEXPR(14)	void preReenter		  (Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void preUpdate		  (Context&)	  noexcept	{}
	FFSM2_CONSTEXPR(14)	void postUpdate		  (Context&)	  noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void preReact		  (const TEvent&,
											   Context&)	  noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void postReact		  (const TEvent&,
											   Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void preExitGuard	  (Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void postExit		  (Context&)	  noexcept	{}

	template <typename T>
	static constexpr StateID stateId()						  noexcept	{ return index<StateList, T>();				}
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct FFSM2_EMPTY_BASES B_<TFirst, TRest...>
	: TFirst
	, B_<TRest...>
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_CONSTEXPR(14)	void widePreEntryGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreEnter		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePreReenter		  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreUpdate		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePostUpdate		  (Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePreReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePostReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreExitGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePostExit		  (Context& context)	  noexcept;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	FFSM2_CONSTEXPR(14)	void entryGuard			  (GuardControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void enter				  (PlanControl&)		  noexcept	{}
	FFSM2_CONSTEXPR(14)	void reenter			  (PlanControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void update				  (FullControl&)		  noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void react				  (const TEvent&,
												   FullControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void exitGuard			  (GuardControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void exit				  (PlanControl&)		  noexcept	{}

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void planSucceeded		  (FullControl&)		  noexcept	{}
	FFSM2_CONSTEXPR(14)	void planFailed			  (FullControl&)		  noexcept	{}
#endif

	FFSM2_CONSTEXPR(14)	void widePreEntryGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreEnter		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePreReenter		  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreUpdate		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePostUpdate		  (Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePreReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePostReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreExitGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePostExit		  (Context& context)	  noexcept;
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreEntryGuard(Context& context) noexcept {
	TF		 ::	   preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreEnter(Context& context) noexcept {
	TF		 ::	   preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreReenter(Context& context) noexcept {
	TF		 ::	   preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreUpdate(Context& context) noexcept {
	TF		 ::	   preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePostUpdate(Context& context) noexcept {
	TF		 ::	   postUpdate(context);
	B_<TR...>::widePostUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreReact(const TEvent& event,
							Context& context) noexcept
{
	TF		 ::	   preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

template <typename TF, typename... TR>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePostReact(const TEvent& event,
							Context& context) noexcept
{
	TF		 ::	   postReact(event, context);
	B_<TR...>::widePostReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreExitGuard(Context& context) noexcept {
	TF		 ::	   preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
FFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePostExit(Context& context) noexcept {
	TF		 ::	   postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePreEntryGuard(Context& context) noexcept {
	TF::	preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePreEnter(Context& context) noexcept {
	TF::	preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePreReenter(Context& context) noexcept {
	TF::	preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePreUpdate(Context& context) noexcept {
	TF::	preUpdate(context);
}

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePostUpdate(Context& context) noexcept {
	TF::	postUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePreReact(const TEvent& event,
					 Context& context) noexcept
{
	TF::	preReact(event, context);
}

template <typename TF>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePostReact(const TEvent& event,
					  Context& context) noexcept
{
	TF::	postReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePreExitGuard(Context& context) noexcept {
	TF::	preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
FFSM2_CONSTEXPR(14)
void
B_<TF>::widePostExit(Context& context) noexcept {
	TF::	postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

//------------------------------------------------------------------------------

#if FFSM2_DEBUG_STATE_TYPE_AVAILABLE() || FFSM2_STRUCTURE_REPORT_AVAILABLE() || FFSM2_LOG_INTERFACE_AVAILABLE()

struct None {};

#endif

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  typename THead>
struct S_
	: THead
{
	static constexpr auto STATE_ID	 = NStateId;

	using Context		= typename TArgs::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TArgs::Logger;
#endif

	using Control		= ControlT<TArgs>;

	using PlanControl	= PlanControlT<TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;

	//----------------------------------------------------------------------
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool	deepEntryGuard		 (GuardControl& control)  noexcept;

	FFSM2_CONSTEXPR(14)	void	deepEnter			 (PlanControl&  control)  noexcept;
	FFSM2_CONSTEXPR(14)	void	deepReenter			 (PlanControl&  control)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	deepUpdate			 (FullControl&  control)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	deepReact			 (FullControl&  control,
													  const TEvent& event)	  noexcept;

	FFSM2_CONSTEXPR(14)	bool	deepExitGuard		 (GuardControl& control)  noexcept;

	FFSM2_CONSTEXPR(14)	void	deepExit			 (PlanControl&  control)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void	wrapPlanSucceeded	 (FullControl&  control)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wrapPlanFailed		 (FullControl&  control)  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	deepChangeToRequested(Control&)				  noexcept	{}

	//----------------------------------------------------------------------

#if FFSM2_DEBUG_STATE_TYPE_AVAILABLE() || FFSM2_STRUCTURE_REPORT_AVAILABLE() || FFSM2_LOG_INTERFACE_AVAILABLE()

	using Empty			= EmptyT<TArgs>;

	static FFSM2_CONSTEXPR(11)	bool isBare() noexcept	{ return std::is_base_of<Head, Empty>::value;	}

	FFSM2_IF_TYPEINDEX(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));

	static constexpr Long NAME_COUNT = isBare() ? 0 : 1;

#endif

	//----------------------------------------------------------------------

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	template <typename TReturn, typename THost, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...),
			 Logger& logger,
			 Context& context,
			 const Method method)										const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 Context&,
			 const Method)												const noexcept
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
bool
S_<NN, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::widePreEntryGuard(control.context());
	Head::		 entryGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepEnter(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreEnter(control.context());
	Head::		 enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepReenter(PlanControl& control) noexcept {
	FFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReenter(control.context());
	Head::		 reenter(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
Status
S_<NN, TA, TH>::deepUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head:: widePreUpdate(control.context());
	Head::		  update(control);
	Head::widePostUpdate(control.context());

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
Status
S_<NN, TA, TH>::deepReact(FullControl& control,
						  const TEvent& event) noexcept
{
	auto reaction = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	FFSM2_LOG_STATE_METHOD(reaction,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head:: widePreReact(event, control.context());
	(this->*reaction)  (event, control);
	Head::widePostReact(event, control.context());

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
bool
S_<NN, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::widePreExitGuard(control.context());
	Head::		 exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepExit(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from ffsm2::Machine::Instance::State
	Head::		  exit(control);
	Head::widePostExit(control.context());

#if FFSM2_PLANS_AVAILABLE()
	control._planData.clearTaskStatus(STATE_ID);
#endif
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planFailed(control);
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct CI_;

template <typename>
struct CSI_;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> final {
	using Type = SI_<TH>;
};

template <typename TH, typename... TS>
struct WrapInfoT<CI_<TH, TS...>> final {
	using Type = CI_<TH, TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------
// COMMON

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;

	static constexpr Short WIDTH		= 1;

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, typename... TR>
struct CSI_<TL_<TI, TR...>> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= CSI_<TL_<TR...>>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

template <typename TI>
struct CSI_<TL_<TI>> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TL_<TSubStates...>>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH		= sizeof...(TSubStates);

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  WIDTH_BITS	= (Long) bitWidth(WIDTH);
	static constexpr Long  ACTIVE_BITS	= WIDTH_BITS;
#endif

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

// COMMON
////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct ArgsT final {
	using Context		= TContext;
	using PureContext	= Undecorate<Context>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short SERIAL_BITS		  = NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload		= TPayload;

#if FFSM2_SERIALIZATION_AVAILABLE()
	using SerialBuffer	= StreamBufferT  <SERIAL_BITS>;
	using WriteStream	= BitWriteStreamT<SERIAL_BITS>;
	using ReadStream	= BitReadStreamT <SERIAL_BITS>;
#endif
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <StateID, typename, Short, typename>
struct CS_;

template <typename, typename>
class RC_;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct MaterialT_;

template <StateID NN, typename TA, typename TH>
struct MaterialT_  <NN, TA, TH> final {
	using Type = S_<NN, TA, TH>;
};

template <StateID NN, typename TA, 			   typename... TS>
struct MaterialT_  <NN, TA, CI_<void,   TS...>> {
	using Type = C_<   TA, EmptyT<TA>, TS...>;
};

template <StateID NN, typename TA, typename TH, typename... TS>
struct MaterialT_  <NN, TA, CI_<TH,	   TS...>> {
	using Type = C_<   TA,	   TH,	   TS...>;
};

template <StateID NN, typename... TS>
using MaterialT = typename MaterialT_<NN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT = TConfig::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY	  = TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload		= typename TConfig::Payload;
	using Transition	= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using Task			= typename TConfig::Task;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
#endif

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  FFSM2_IF_SERIALIZATION(, ACTIVE_BITS)
							  , SUBSTITUTION_LIMIT
							  FFSM2_IF_PLANS(, TASK_CAPACITY)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RC_<TConfig, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	//----------------------------------------------------------------------

	using State			= EmptyT<Args>;

	template <typename... TInjections>
	using StateT		= B_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	template <typename T>
	static constexpr bool	 contains() noexcept	{ return contains<StateList, T>();	}

	template <typename T>
	static constexpr StateID  stateId() noexcept	{ return index	 <StateList, T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, Short, typename>
struct LHalfCST;

template <StateID NN, typename TA, Short NI, typename... TS>
struct LHalfCST<NN, TA, NI, TL_<TS...>> final {
	using Type = CS_<NN,
					 TA,
					 NI,
					 LHalfTypes<TS...>>;
};

template <StateID NN, typename TA, Short NI, typename TL>
using LHalfCS = typename LHalfCST<NN, TA, NI, TL>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID, typename, Short, typename>
struct RHalfCST;

template <StateID NN, typename TA, Short NI, typename... TS>
struct RHalfCST<NN, TA, NI, TL_<TS...>> final {
	using Type = CS_<NN + sizeof...(TS) / 2,
					 TA,
					 NI + sizeof...(TS) / 2,
					 RHalfTypes<TS...>>;
};

template <StateID NN, typename TA, Short NI, typename TL>
using RHalfCS = typename RHalfCST<NN, TA, NI, TL>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename... TStates>
struct FFSM2_EMPTY_BASES CS_<NStateId,
							 TArgs,
							 NIndex,
							 TL_<TStates...>>
	: LHalfCS<NStateId,
			  TArgs,
			  NIndex,
			  TL_<TStates...>>
	, RHalfCS<NStateId,
			  TArgs,
			  NIndex,
			  TL_<TStates...>>
{
	static_assert(sizeof...(TStates) >= 2, "");

	using Args			= TArgs;
	using SubStateList	= TL_<TStates...>;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr StateID  INITIAL_ID  = NStateId;
	static constexpr Short	  PRONG_INDEX = NIndex;
	static constexpr Short	  R_PRONG	  = PRONG_INDEX + sizeof...(TStates) / 2;

	using LHalf			= LHalfCS<INITIAL_ID,
								  Args,
								  PRONG_INDEX,
								  SubStateList>;

	using RHalf			= RHalfCS<INITIAL_ID,
								  Args,
								  PRONG_INDEX,
								  SubStateList>;

	//----------------------------------------------------------------------
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter			 (PlanControl&  control, const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter			 (PlanControl&  control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	wideUpdate			 (FullControl&  control, const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	wideReact			 (FullControl&  control,
													  const TEvent& event,	 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit			 (PlanControl&  control, const Short prong)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideChangeToRequested(PlanControl&  control, const Short prong)  noexcept;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename TState>
struct CS_<NStateId,
		   TArgs,
		   NIndex,
		   TL_<TState>>
	: MaterialT<NStateId,
				TArgs,
				TState>
{
	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= MaterialT<NStateId,
									Args,
									TState>;

	//----------------------------------------------------------------------
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter			 (PlanControl&  control, const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter			 (PlanControl&  control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	wideUpdate			 (FullControl&  control, const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	wideReact			 (FullControl&  control,
													  const TEvent& event,	 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit			 (PlanControl&  control, const Short prong)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideChangeToRequested(PlanControl&  control, const Short prong)  noexcept;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<TS...>>::wideEntryGuard(GuardControl& control,
											const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideEnter(PlanControl& control,
									   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideReenter(PlanControl& control,
										 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<TS...>>::wideUpdate(FullControl& control,
										const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<TS...>>::wideReact(FullControl& control,
									   const TEvent& event,
									   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<TS...>>::wideExitGuard(GuardControl& control,
										   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideExit(PlanControl& control,
									  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideChangeToRequested(PlanControl& control,
												   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<T>>::wideEntryGuard(GuardControl& control,
										const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideEnter(PlanControl& control,
								   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideReenter(PlanControl& control,
									 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<T>>::wideUpdate(FullControl& control,
									const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<T>>::wideReact(FullControl& control,
								   const TEvent& event,
								   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepReact(control, event);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<T>>::wideExitGuard(GuardControl& control,
									   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideExit(PlanControl& control,
								  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepExit(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct FFSM2_EMPTY_BASES C_
	: S_<INVALID_STATE_ID, TArgs, THead>
	, CS_<0, TArgs, 0, TL_<TSubStates...>>
{
	using Args			= TArgs;

	using HeadState		= S_<INVALID_STATE_ID, Args, THead>;
	using SubStates		= CS_<0, Args, 0, TL_<TSubStates...>>;

	using StateList		= typename Args::StateList;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;


#if FFSM2_SERIALIZATION_AVAILABLE()
	using Head			= THead;
	using Info			= CI_<Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(14)	bool deepForwardEntryGuard(GuardControl& control)  noexcept;
	FFSM2_CONSTEXPR(14)	bool deepEntryGuard		  (GuardControl& control)  noexcept;

	FFSM2_CONSTEXPR(14)	void deepEnter			  (PlanControl&  control)  noexcept;
	FFSM2_CONSTEXPR(14)	void deepReenter		  (PlanControl&  control)  noexcept;

	FFSM2_CONSTEXPR(14)	void deepUpdate			  (FullControl&  control)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void deepReact			  (FullControl&  control,
												   const TEvent& event)    noexcept;

	FFSM2_CONSTEXPR(14)	bool deepForwardExitGuard (GuardControl& control)  noexcept;
	FFSM2_CONSTEXPR(14)	bool deepExitGuard		  (GuardControl& control)  noexcept;

	FFSM2_CONSTEXPR(14)	void deepExit			  (PlanControl&  control)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void deepChangeToRequested(PlanControl&  control)  noexcept;

	//----------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	FFSM2_CONSTEXPR(14)	void deepSaveActive		  (const Registry& registry, WriteStream& stream) const noexcept;
	FFSM2_CONSTEXPR(14)	void deepLoadRequested	  (		 Registry& registry, ReadStream&  stream) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.active != INVALID_SHORT);

	const Short  requested  = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return SubStates::wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   == INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	active	  = requested;
	requested = INVALID_SHORT;

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepReenter(PlanControl& /*control*/) noexcept {
}

//------------------------------------------------------------------------------
// COMMON

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (HeadState::deepUpdate(control)) {
		ControlLock lock{control};

		SubStates::wideUpdate(control, active);
	} else {
		FFSM2_IF_PLANS(const Status subStatus =)
		SubStates::wideUpdate(control, active);

	#if FFSM2_PLANS_AVAILABLE()
		if (subStatus && control._planData.planExists)
			control.updatePlan((HeadState&) *this, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepReact(FullControl& control,
							 const TEvent& event) noexcept
{
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (HeadState::deepReact(control, event)) {
		ControlLock lock{control};

		SubStates::wideReact(control, event, active);
	} else {
		FFSM2_IF_PLANS(const Status subStatus =)
		SubStates::wideReact(control, event, active);

	#if FFSM2_PLANS_AVAILABLE()
		if (subStatus && control._planData.planExists)
			control.updatePlan((HeadState&) *this, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	const Short  active  = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	return SubStates::wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	active = INVALID_SHORT;

#if FFSM2_PLANS_AVAILABLE()
	auto plan = control.plan();
	plan.clear();
#endif
}

// COMMON
//------------------------------------------------------------------------------
// COMMON

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   != INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	if (requested != active) {
		SubStates::wideExit	  (control, active);

		active	  = requested;
		requested = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	} else {
		requested = INVALID_SHORT;

		// reconstruction done in S_::reenter()
		SubStates::wideReenter(control, active);
	}
}

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepSaveActive(const Registry& registry,
								  WriteStream& stream) const noexcept
{
	stream.template write<WIDTH_BITS>(registry.active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepLoadRequested(Registry& registry,
									 ReadStream& stream) const noexcept
{
	registry.requested = stream.template read<WIDTH_BITS>();
	FFSM2_ASSERT(registry.requested < WIDTH);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct G_ final {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;
	using Activation		 = TActivation;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long TASK_CAPACITY		 = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , Activation, SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Select manual activation strategy
	using ManualActivation	 = G_<FEATURE_TAG, Context, Manual	  , SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, Activation, N				   FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context, Activation, SUBSTITUTION_LIMIT				  , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, Activation, SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), T      >;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>> final {
	using Cfg = G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root				= RF_<Cfg, CI_<THead, TSubStates...>>;

	/// @brief Headless root
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot			= RF_<Cfg, CI_<void,  TSubStates...>>;

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif
};

////////////////////////////////////////////////////////////////////////////////

}

/// @brief Type configuration for MachineT<>
using Config = detail::G_<FFSM2_FEATURE_TAG, EmptyContext, Automatic, 4 FFSM2_IF_PLANS(, INVALID_LONG), void>;

/// @brief 'Template namespace' for FSM classes
/// @tparam TConfig 'ConfigT<>' type configuration for MachineT<>
/// @see ConfigT<>
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

////////////////////////////////////////////////////////////////////////////////

}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class R_ {
public:
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;
	using Payload				= typename TConfig::Payload;

protected:
	using Forward				= RF_<TConfig, TApex>;
	using StateList				= typename Forward::StateList;

	using Args					= typename Forward::Args;
	using PureContext			= typename Args::PureContext;

	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	using Apex					= MaterialT<0, Args, TApex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	/// @brief Transition
	using Transition			= typename Control::Transition;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)	explicit R_(Context& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(11)	explicit R_(const PureContext& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(11)	explicit R_(PureContext&& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(11) R_(const R_&  other)												  noexcept;
	FFSM2_CONSTEXPR(11) R_(		 R_&& other)												  noexcept;

	FFSM2_CONSTEXPR(20)	~R_()																  noexcept;

	//----------------------------------------------------------------------

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(14)		  Context& context()											  noexcept	{ return _context;								}

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(11)	const Context& context()										const noexcept	{ return _context;								}

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()														  noexcept	{ return index<StateList, TState>();			}

	//----------------------------------------------------------------------

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(14)		  TState& access()												  noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(11)	const TState& access()											const noexcept	{ return static_cast<const TState&>(_apex);		}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	FFSM2_CONSTEXPR(14)	void update()														  noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void react(const TEvent& event)										  noexcept;

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_CONSTEXPR(11)	StateID activeStateId()											const noexcept	{ return _registry.active;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive(const StateID stateId)							const noexcept	{ return _registry.active == stateId;			}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive()													const noexcept	{ return _registry.active == stateId<TState>();	}

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	FFSM2_CONSTEXPR(14)	void changeTo		 (const StateID stateId)						  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo		 ()												  noexcept	{ changeTo (stateId<TState>());					}

	// COMMON
	//----------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	/// @see FFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer 'SerialBuffer' to serialize to
	/// @see FFSM2_ENABLE_SERIALIZATION
	FFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer 'SerialBuffer' to de-serialize from
	/// @see FFSM2_ENABLE_SERIALIZATION
	FFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)								  noexcept;

#endif

	//----------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the transition recorded during last 'update()' / 'react()'
	/// @return Array of last recorded transitions
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(11)	const Transition& previousTransition()							const noexcept	{ return _previousTransition;					}

	/// @brief Force process a transition (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param destination Transition destination
	/// @return Success status
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(14)	bool replayTransition(const StateID destination)					  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

	//----------------------------------------------------------------------

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)								  noexcept	{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

protected:
	FFSM2_CONSTEXPR(14)	void initialEnter()													  noexcept;
	FFSM2_CONSTEXPR(14)	void finalExit()													  noexcept;

	FFSM2_CONSTEXPR(14)	void processTransitions(Transition& currentTransition)				  noexcept;

	FFSM2_CONSTEXPR(14)	bool applyRequest(const Transition& currentTransition,
										  const StateID destination)						  noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByEntryGuards(const Transition& currentTransition,
													const Transition& pendingTransition)	  noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByGuards(const Transition& currentTransition,
											   const Transition& pendingTransition)			  noexcept;

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	Transition _previousTransition;
#endif

	Context _context;

	Registry _registry;
	FFSM2_IF_PLANS(PlanData _planData);

	Transition _request;

	Apex _apex;

	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

// Automatic / manual [de]activation

template <typename, typename>
class RV_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Automatic enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	FFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	FFSM2_CONSTEXPR(14)	RV_(const RV_&  other)												  noexcept;
	FFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)												  noexcept;

	FFSM2_CONSTEXPR(20)	~RV_()																  noexcept;

private:
	using Base::initialEnter;
	using Base::finalExit;

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_apex;
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Manual enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, Manual, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, Manual, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, Manual, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	using typename Base::Transition;

private:
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
#endif

public:
	using Base::Base;

	/// @brief Manually start the FSM
	///   Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	FFSM2_CONSTEXPR(14)	void enter()														  noexcept	{ initialEnter();	}

	/// @brief Manually stop the FSM
	///   Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	FFSM2_CONSTEXPR(14)	void exit()															  noexcept	{ finalExit();		}

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///   Can be used with UE4 USTRUCT() NetSerialize() to load replicated FSM from FArchive
	/// @param destination Transition destination
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(14)	void replayEnter(const StateID destination)							  noexcept;

#endif

private:
	using Base::initialEnter;
	using Base::finalExit;

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Apex					= typename Base::Apex;

	using Base::applyRequest;

	using Base::_previousTransition;

	using Base::_context;
	using Base::_apex;
	using Base::_registry;
	#if FFSM2_PLANS_AVAILABLE()
		using Base::_planData;
	#endif
	using Base::_request;
	#if FFSM2_LOG_INTERFACE_AVAILABLE()
		using Base::_logger;
	#endif
#endif
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Non-'void' payloads

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Transition			= TransitionT<TPayload>;

public:
	using typename Base::Payload;

public:
	using Base::Base;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()														  noexcept	{ return Base::template stateId<TState>();		}

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith(const StateID stateId,
										const Payload &payload)								  noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith(const StateID stateId,
											Payload&& payload)								  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)								  noexcept	{ changeWith(stateId<TState>(),		 payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(Payload&& payload)									  noexcept	{ changeWith(stateId<TState>(), move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

protected:
	using Base::_context;
	using Base::_registry;

private:
	using Base::_request;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Base::_logger;
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename, typename>
class RC_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit RC_(Context& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	FFSM2_CONSTEXPR(11)	explicit RC_(PureContext&& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	using Base::Base;

	FFSM2_CONSTEXPR(14)	void setContext(const Context&  context)							  noexcept	{ _context =	  context ;	}
	FFSM2_CONSTEXPR(14)	void setContext(	  Context&& context)							  noexcept	{ _context = move(context);	}

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext&

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext&, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, TContext&, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext&, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	using Base::Base;

	FFSM2_CONSTEXPR(14)	void setContext(Context context)									  noexcept	{ _context = context; }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext*

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext*, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, TContext*, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext*, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit RC_(Context context = nullptr
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	FFSM2_CONSTEXPR(14)	void setContext(Context context)									  noexcept	{ _context = context; }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class FFSM2_EMPTY_BASES RC_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public			RP_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, EmptyContext
{
	using Base		  = RP_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit RC_(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))  noexcept;

	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(Context& context
			 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(const PureContext& context
			 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(PureContext&& context
			 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{move(context)}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(const R_&  other) noexcept
	: _context {other._context }
	, _registry{other._registry}
#if FFSM2_PLANS_AVAILABLE()
	, _planData{other._planData}
#endif
	, _request {other._request }
#if FFSM2_LOG_INTERFACE_AVAILABLE()
	, _logger  {other._logger  }
#endif
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(R_&& other) noexcept
	: _context {move(other._context )}
	, _registry{move(other._registry)}
#if FFSM2_PLANS_AVAILABLE()
	, _planData{move(other._planData)}
#endif
	, _request {move(other._request )}
#if FFSM2_LOG_INTERFACE_AVAILABLE()
	, _logger  {move(other._logger  )}
#endif
{}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(20)
R_<TG, TA>::~R_() noexcept {
	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::update() noexcept {
	FFSM2_ASSERT(_registry.isActive());

	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

	Transition currentTransition;

	if (_request)
		processTransitions(currentTransition);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::react(const TEvent& event) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

	Transition currentTransition;

	if (_request)
		processTransitions(currentTransition);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::changeTo(const StateID stateId) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::save(SerialBuffer& _buffer) const noexcept {
	FFSM2_ASSERT(_registry.isActive());

	WriteStream stream{_buffer};

	// TODO: save _registry
	// TODO: save _requests
	// TODO: save _rng						// FFSM2_IF_UTILITY_THEORY()
	// TODO: save _planData					// FFSM2_IF_PLANS()
	// TODO: save _previousTransition		// FFSM2_IF_TRANSITION_HISTORY()
	// TODO: save _activityHistory			// FFSM2_IF_STRUCTURE_REPORT()

	_apex.deepSaveActive(_registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::load(const SerialBuffer& buffer) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_request.clear();

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit(control);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition.clear());

	_registry.clearRequests();
	_request.clear();

	ReadStream stream{buffer};

	// TODO: load _registry
	// TODO: load _requests
	// TODO: load _rng					// FFSM2_IF_UTILITY_THEORY()
	// TODO: load _planData				// FFSM2_IF_PLANS()
	// TODO: load _previousTransition	// FFSM2_IF_TRANSITION_HISTORY()
	// TODO: load _activityHistory		// FFSM2_IF_STRUCTURE_REPORT()

	_apex.deepLoadRequested(_registry, stream);

	_apex.deepEnter(control);
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransition(const StateID destination) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_previousTransition.clear();

	if (FFSM2_CHECKED(destination != INVALID_SHORT)) {
		PlanControl control{_context
						  , _registry
						  , _request
						  FFSM2_IF_PLANS(, _planData)
						  , _previousTransition
						  FFSM2_IF_LOG_INTERFACE(, _logger)};

		Transition currentTransition;
		applyRequest(currentTransition,
					 destination);

		_previousTransition = Transition{destination};

		_apex.deepChangeToRequested(control);

		_registry.clearRequests();

		FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

		return true;
	}

	return false;
}

#endif

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::initialEnter() noexcept {
	FFSM2_ASSERT(!_registry.isActive());
	FFSM2_ASSERT(!_request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_previousTransition));

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Transition currentTransition;
	Transition pendingTransition;

	applyRequest(currentTransition, 0);

	cancelledByEntryGuards(currentTransition,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _request;
		 ++i)
	{
		//backup();

		if (applyRequest(currentTransition,
						 _request.destination))
		{
			pendingTransition = _request;
			_request.clear();

			if (cancelledByEntryGuards(currentTransition,
									   pendingTransition))
				FFSM2_BREAK();
			else
				currentTransition = pendingTransition;

			pendingTransition.clear();
		} else
			_request.clear();
	}
	FFSM2_ASSERT(!_request);
	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);

	_apex.deepEnter(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::finalExit() noexcept {
	FFSM2_ASSERT(_registry.isActive());
	FFSM2_ASSERT(!_request);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processTransitions(Transition& currentTransition) noexcept {
	FFSM2_ASSERT(_request);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Transition pendingTransition;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _request;
		++i)
	{
		//backup();

		if (applyRequest(currentTransition,
						 _request.destination))
		{
			pendingTransition = _request;
			_request.clear();

			if (cancelledByGuards(currentTransition,
								  pendingTransition))
				;
			else
				currentTransition = pendingTransition;

			pendingTransition.clear();
		} else
			_request.clear();
	}
	FFSM2_ASSERT(!_request);

	if (currentTransition)
		_apex.deepChangeToRequested(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequest(const Transition& currentTransition,
						 const StateID destination) noexcept
{
	if (currentTransition != Transition{destination}) {
		_registry.requested = destination;

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByEntryGuards(const Transition& currentTransition,
								   const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransition
							, pendingTransition
							FFSM2_IF_PLANS(, _planData)
							FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByGuards(const Transition& currentTransition,
							  const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransition
							, pendingTransition
							FFSM2_IF_PLANS(, _planData)
							FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard (guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(Context& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(PureContext&& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(const RV_& other) noexcept
	: Base{other}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(RV_&& other) noexcept
	: Base{move(other)}
{
	initialEnter();
}

//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(20)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::~RV_() noexcept {
	finalExit();
}

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const StateID destination) noexcept {
	FFSM2_ASSERT(_registry.active == INVALID_SHORT);
	FFSM2_ASSERT(!_request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_previousTransition));

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Transition currentTransition;
	applyRequest(currentTransition,
				 destination);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = Transition{destination});

	_apex.deepEnter(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																	const Payload& payload) noexcept
{
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId, payload};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																		 Payload&& payload) noexcept
{
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId, move(payload)};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

// COMMON
//------------------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
RC_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(Context& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
RC_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(PureContext&& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
RC_<G_<NFT, TC*, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(Context context
															FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
//FFSM2_CONSTEXPR(11)
//RC_<G_<NFT, TC*, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(const Context& context
//															FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
//	: Base{move(context)
//	FFSM2_IF_LOG_INTERFACE(, logger)}
//{}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
RC_<G_<NFT, EmptyContext, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(FFSM2_IF_LOG_INTERFACE(Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

}
}

//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

#if _MSC_VER == 1900
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

//#undef FFSM2_UNUSED

#undef FFSM2_ATTRIBUTE
#undef FFSM2_ATTRIBUTE_FALLTHROUGH
#undef FFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS

#undef FFSM2_CONSTEXPR
#undef FFSM2_CONSTEXPR_NO
#undef FFSM2_CONSTEXPR_11
#undef FFSM2_CONSTEXPR_14
#undef FFSM2_CONSTEXPR_17
#undef FFSM2_CONSTEXPR_20

#undef FFSM2_ARCHITECTURE
#undef FFSM2_ARCHITECTURE_64
#undef FFSM2_ARCHITECTURE_32
#undef FFSM2_64BIT_OR_32BIT

//#undef FFSM2_BREAK
#undef FFSM2_BREAK_AVAILABLE

#undef FFSM2_IF_DEBUG
#undef FFSM2_UNLESS_DEBUG
#undef FFSM2_DEBUG_OR

#undef FFSM2_ASSERT_AVAILABLE
#undef FFSM2_IF_ASSERT
//#undef FFSM2_CHECKED
#undef FFSM2_ASSERT
#undef FFSM2_ASSERT_OR

#undef FFSM2_IF_TYPEINDEX
#undef FFSM2_TYPEINDEX_AVAILABLE
#undef FFSM2_IF_TYPEINDEX

//#undef FFSM2_DEBUG_STATE_TYPE_AVAILABLE

//#undef FFSM2_PLANS_AVAILABLE
#undef FFSM2_IF_PLANS

//#undef FFSM2_SERIALIZATION_AVAILABLE
#undef FFSM2_IF_SERIALIZATION

//#undef FFSM2_STRUCTURE_REPORT_AVAILABLE
//#undef FFSM2_IF_STRUCTURE_REPORT

//#undef FFSM2_TRANSITION_HISTORY_AVAILABLE
#undef FFSM2_IF_TRANSITION_HISTORY

#undef FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE
#undef FFSM2_LOG_INTERFACE_AVAILABLE
#undef FFSM2_IF_LOG_INTERFACE

#undef FFSM2_LOG_TRANSITION

#if FFSM2_PLANS_AVAILABLE()
	#undef FFSM2_LOG_TASK_STATUS
	#undef FFSM2_LOG_PLAN_STATUS
#endif

#undef FFSM2_LOG_CANCELLED_PENDING

#undef FFSM2_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////
