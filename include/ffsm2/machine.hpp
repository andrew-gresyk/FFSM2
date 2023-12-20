// FFSM2 (flat state machine for games and interactive applications)
// 2.3.3 (2023-12-21)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2023
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

#define FFSM2_VERSION_MAJOR 2
#define FFSM2_VERSION_MINOR 3
#define FFSM2_VERSION_PATCH 3

#define FFSM2_VERSION (10000 * FFSM2_VERSION_MAJOR + 100 * FFSM2_VERSION_MINOR + FFSM2_VERSION_PATCH)

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef FFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

#define FFSM2_UNUSED(x)

#define FFSM2_ATTRIBUTE(A)								   FFSM2_ATTRIBUTE_##A()

#ifndef __has_cpp_attribute
	#define __has_cpp_attribute(x)											   0
#endif

#if __has_cpp_attribute(fallthrough)
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()						 [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()				  [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()					[[gnu::fallthrough]]
#else
	#define FFSM2_ATTRIBUTE_FALLTHROUGH()
#endif

#if __has_cpp_attribute(no_unique_address)
	#define FFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()			   [[no_unique_address]]
#else
	#define FFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()
#endif

#define FFSM2_CONSTEXPR(A)								   FFSM2_CONSTEXPR_##A()

#define FFSM2_CONSTEXPR_NO()

#define FFSM2_CONSTEXPR_11()										   constexpr

#if __cplusplus >= 201402L
	#define FFSM2_CONSTEXPR_14()									   constexpr
#else
	#define FFSM2_CONSTEXPR_14()										  inline
#endif

#if __cplusplus >= 201703L
	#define FFSM2_CONSTEXPR_17()									   constexpr
#else
	#define FFSM2_CONSTEXPR_17()										  inline
#endif

#if __cplusplus >= 202002L
	#define FFSM2_CONSTEXPR_20()									   constexpr
#else
	#define FFSM2_CONSTEXPR_20()										  inline
#endif

#if defined _MSC_VER
	#define FFSM2_EMPTY_BASES							 __declspec(empty_bases)
#else
	#define FFSM2_EMPTY_BASES
#endif

#if defined _DEBUG && defined _MSC_VER
	#define FFSM2_BREAK()										  __debugbreak()
	#define FFSM2_BREAK_AVAILABLE()											true
#else
	#define FFSM2_BREAK()											  ((void) 0)
	#define FFSM2_BREAK_AVAILABLE()										   false
#endif

#ifdef _DEBUG
	#define FFSM2_IF_DEBUG(...)										 __VA_ARGS__
	#define FFSM2_UNLESS_DEBUG(...)
	#define FFSM2_DEBUG_OR(y, n)											   y
#else
	#define FFSM2_IF_DEBUG(...)
	#define FFSM2_UNLESS_DEBUG(...)									 __VA_ARGS__
	#define FFSM2_DEBUG_OR(y, n)											   n
#endif

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

#ifdef FFSM2_ENABLE_ALL
	#define FFSM2_ENABLE_DEBUG_STATE_TYPE
	#define FFSM2_ENABLE_PLANS
	#define FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_ENABLE_STRUCTURE_REPORT
	#define FFSM2_ENABLE_TRANSITION_HISTORY
#endif

#ifndef FFSM2_DISABLE_TYPEINDEX
	#define FFSM2_TYPEINDEX_AVAILABLE()										true
	#define FFSM2_IF_TYPEINDEX(...)									 __VA_ARGS__
	#define FFSM2_TYPEINDEX_MASK										(1 << 0)
#else
	#define FFSM2_TYPEINDEX_AVAILABLE()									   false
	#define FFSM2_IF_TYPEINDEX(...)
	#define FFSM2_TYPEINDEX_MASK										(0 << 0)
#endif

#ifdef FFSM2_ENABLE_DEBUG_STATE_TYPE
	#define FFSM2_DEBUG_STATE_TYPE_AVAILABLE()								true
	#define FFSM2_DEBUG_STATE_TYPE_MASK									(1 << 1)
#else
	#define FFSM2_DEBUG_STATE_TYPE_AVAILABLE()							   false
	#define FFSM2_DEBUG_STATE_TYPE_MASK									(0 << 1)
#endif

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_PLANS_AVAILABLE()											true
	#define FFSM2_IF_PLANS(...)										 __VA_ARGS__
	#define FFSM2_PLANS_MASK											(1 << 2)
#else
	#define FFSM2_PLANS_AVAILABLE()										   false
	#define FFSM2_IF_PLANS(...)
	#define FFSM2_PLANS_MASK											(0 << 2)
#endif

#ifdef FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_SERIALIZATION_AVAILABLE()									true
	#define FFSM2_IF_SERIALIZATION(...)								 __VA_ARGS__
	#define FFSM2_SERIALIZATION_MASK									(1 << 3)
#else
	#define FFSM2_SERIALIZATION_AVAILABLE()								   false
	#define FFSM2_IF_SERIALIZATION(...)
	#define FFSM2_SERIALIZATION_MASK									(0 << 3)
#endif

#ifdef FFSM2_ENABLE_STRUCTURE_REPORT
	#define FFSM2_STRUCTURE_REPORT_AVAILABLE()								true
	#define FFSM2_IF_STRUCTURE_REPORT(...)							 __VA_ARGS__
	#define FFSM2_STRUCTURE_REPORT_MASK									(1 << 4)
#else
	#define FFSM2_STRUCTURE_REPORT_AVAILABLE()							   false
	#define FFSM2_IF_STRUCTURE_REPORT(...)
	#define FFSM2_STRUCTURE_REPORT_MASK									(0 << 4)
#endif

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	#define FFSM2_TRANSITION_HISTORY_AVAILABLE()							true
	#define FFSM2_IF_TRANSITION_HISTORY(...)						 __VA_ARGS__
	#define FFSM2_TRANSITION_HISTORY_MASK								(1 << 5)
#else
	#define FFSM2_TRANSITION_HISTORY_AVAILABLE()						   false
	#define FFSM2_IF_TRANSITION_HISTORY(...)
	#define FFSM2_TRANSITION_HISTORY_MASK								(0 << 5)
#endif

#ifdef FFSM2_ENABLE_UTILITY_THEORY
	#define FFSM2_UTILITY_THEORY_AVAILABLE()								true
#else
	#define FFSM2_UTILITY_THEORY_AVAILABLE()							   false
#endif

#ifdef FFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define FFSM2_ENABLE_LOG_INTERFACE

	#define FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()								true
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								(1 << 7)
#else
	#define FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()							   false
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								(0 << 7)
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	#define FFSM2_LOG_INTERFACE_AVAILABLE()									true
	#define FFSM2_IF_LOG_INTERFACE(...)								 __VA_ARGS__
	#define FFSM2_LOG_INTERFACE_MASK									(1 << 8)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)				   \
		if (_core.logger)													   \
			_core.logger->recordTransition(CONTEXT, ORIGIN, DESTINATION)

#if FFSM2_PLANS_AVAILABLE()
	#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)					   \
		if (_core.logger)													   \
			_core.logger->recordTaskStatus(CONTEXT, ORIGIN, STATUS)

	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)							   \
		if (_core.logger)													   \
			_core.logger->recordPlanStatus(CONTEXT, STATUS)
#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)					   \
		if (_core.logger)													   \
			_core.logger->recordCancelledPending(CONTEXT, ORIGIN)

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

#if FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._core.logger)						   \
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

	#define FFSM2_IF_LOG_STATE_METHOD(...)							 __VA_ARGS__

#elif FFSM2_LOG_INTERFACE_AVAILABLE()

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._core.logger)						   \
			log(METHOD, *logger, control.context(), METHOD_ID)

	#define FFSM2_IF_LOG_STATE_METHOD(...)							 __VA_ARGS__

#else

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

	#define FFSM2_IF_LOG_STATE_METHOD(...)

#endif

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

#undef FFSM2_TYPEINDEX_MASK
#undef FFSM2_DEBUG_STATE_TYPE_MASK
#undef FFSM2_PLANS_MASK
#undef FFSM2_SERIALIZATION_MASK
#undef FFSM2_STRUCTURE_REPORT_MASK
#undef FFSM2_TRANSITION_HISTORY_MASK
#undef FFSM2_VERBOSE_DEBUG_LOG_MASK
#undef FFSM2_LOG_INTERFACE_MASK

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

namespace ffsm2 {

struct EmptyPayload final {};

struct Automatic;
struct Manual;

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using Prong		 = Short;
static constexpr Prong		INVALID_PRONG		= INVALID_SHORT;

using Long		 = uint8_t;
static constexpr Long		INVALID_LONG		= UINT8_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

template <
	bool B
  , typename TT
  , typename TF
>
struct ConditionalT final {
	using Type = TT;
};

template <
	typename TT
  , typename TF
>
struct ConditionalT<false, TT, TF> final {
	using Type = TF;
};

template <
	bool B
  , typename TT
  , typename TF
>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

template <typename, typename>
struct IsSameT final {
	static constexpr bool Value = false;
};

template <typename T>
struct IsSameT<T, T> final {
	static constexpr bool Value = true;
};

template <typename T>
struct RemoveConstT final {
	using Type = T;
};

template <typename T>
struct RemoveConstT<const T> final {
	using Type = T;
};

template <typename T>
using RemoveConst = typename RemoveConstT<T>::Type;

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

template <typename T>
using RemoveReference = typename RemoveReferenceT<T>::Type;

template <typename T>
using Undecorate = RemoveConst<RemoveReference<T>>;

template <typename T>
struct IsValueReferenceT final {
	static const bool VALUE = false;
};

template <typename T>
struct IsValueReferenceT<T&> final {
	static const bool VALUE = true;
};

template <uint64_t N>
using UIndex = Conditional<N <= (1ull <<  8),  uint8_t,
			   Conditional<N <= (1ull << 16), uint16_t,
			   Conditional<N <= (1ull << 32), uint32_t,
												  void>>>;

template <uint64_t N>
using UCapacity = Conditional<N < (1ull <<  8),  uint8_t,
				  Conditional<N < (1ull << 16), uint16_t,
				  Conditional<N < (1ull << 32), uint32_t,
													void>>>;

FFSM2_CONSTEXPR(11)
uint32_t
bitWidth(const uint32_t v)												noexcept	{
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

template <uint64_t N>
using UBitWidth = Conditional<N <=  8,  uint8_t,
				  Conditional<N <= 16, uint16_t,
				  Conditional<N <= 32, uint32_t,
										   void>>>;

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>& t)											noexcept	{
	return static_cast<T&&>(t);
}

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>&& t)											noexcept	{
	static_assert(!IsValueReferenceT<T>::VALUE, "");

	return static_cast<T&&>(t);
}

template <typename T>
FFSM2_CONSTEXPR(11)
RemoveReference<T>&&
move(T&& t)																noexcept	{
	return static_cast<RemoveReference<T>&&>(t);
}

template <typename T0,
		  typename T1>
FFSM2_CONSTEXPR(11)
T0
min(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 < static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

template <typename T0,
		  typename T1>
FFSM2_CONSTEXPR(11)
T0
max(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 > static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

template <typename TIndex,
		  typename TElement,
		  TIndex NCount>
FFSM2_CONSTEXPR(11)
TIndex
count(const TElement(&)[NCount])										noexcept	{
	return NCount;
}

template <typename T,
		  typename TT>
FFSM2_CONSTEXPR(11)
T
contain(const T x,
		const TT to)													noexcept
{
	return (x + static_cast<T>(to) - 1) / static_cast<T>(to);
}

template <typename T>
FFSM2_CONSTEXPR(14)
void
fill(T& a, const char value)											noexcept	{
	memset(&a, static_cast<int>(value), sizeof(a));
}

template <int>
struct StaticPrintConstT;

template <typename>
struct StaticPrintTypeT;

}

namespace ffsm2 {
namespace detail {

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class DynamicArrayT;

private:
	FFSM2_CONSTEXPR(11)	IteratorT(Container& container,
								  const Index cursor)										noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& FFSM2_IF_ASSERT(other))	  const noexcept	{
		FFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	FFSM2_CONSTEXPR(14)	IteratorT& operator ++()											noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	FFSM2_CONSTEXPR(14)		  Item& operator *()			noexcept	{ return  _container[_cursor];	}
	FFSM2_CONSTEXPR(11)	const Item& operator *()	  const noexcept	{ return  _container[_cursor];	}

	FFSM2_CONSTEXPR(14)		  Item* operator->()			noexcept	{ return &_container[_cursor];	}
	FFSM2_CONSTEXPR(11)	const Item* operator->()	  const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Index _cursor;
};

template <typename TContainer>
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class DynamicArrayT;

private:
	FFSM2_CONSTEXPR(11)	IteratorT(const Container& container,
								  const Index cursor)										noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& FFSM2_IF_ASSERT(other))	  const noexcept	{
		FFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	FFSM2_CONSTEXPR(14)	IteratorT& operator ++()											noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	FFSM2_CONSTEXPR(11)	const Item& operator *()	  const noexcept	{ return _container[_cursor];	}

	FFSM2_CONSTEXPR(11)	const Item* operator->()	  const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

}
}

#if FFSM2_SERIALIZATION_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <Long>
class BitWriteStreamT;

template <Long>
class BitReadStreamT;

template <Long NBitCapacity>
class StreamBufferT {
	template <Long>
	friend class BitWriteStreamT;

	template <Long>
	friend class BitReadStreamT;

public:
	static constexpr Long BIT_CAPACITY	= NBitCapacity;
	static constexpr Long BYTE_COUNT	= contain(BIT_CAPACITY, 8u);

	using StreamBuffer	= StreamBufferT<BIT_CAPACITY>;
	using Data			= uint8_t	   [BYTE_COUNT	];

	FFSM2_CONSTEXPR(14)	void clear()									noexcept	{ fill(_data, 0);	}

	FFSM2_CONSTEXPR(14)		  Data& data()								noexcept	{ return _data;		}
	FFSM2_CONSTEXPR(11)	const Data& data()						  const noexcept	{ return _data;		}

	FFSM2_CONSTEXPR(14) bool operator == (const StreamBuffer& s)  const noexcept;
	FFSM2_CONSTEXPR(14) bool operator != (const StreamBuffer& s)  const noexcept;

private:
	Data _data {};
};

template <Long NBitCapacity>
class BitWriteStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	FFSM2_CONSTEXPR(14)	explicit BitWriteStreamT(Buffer& buffer,
												 const Long cursor = 0)	noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{
		_buffer.clear();
	}

	template <Short NBitWidth>
	FFSM2_CONSTEXPR(14)	void write(const UBitWidth<NBitWidth> item)		noexcept;

	FFSM2_CONSTEXPR(11)	Long cursor()							  const noexcept	{ return _cursor;	}

private:
	Buffer& _buffer;

	Long _cursor = 0;
};

template <Long NBitCapacity>
class BitReadStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	FFSM2_CONSTEXPR(11)	explicit BitReadStreamT(const Buffer& buffer,
												const Long cursor = 0)	noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{}

	template <Short NBitWidth>
	FFSM2_CONSTEXPR(14)	UBitWidth<NBitWidth> read()						noexcept;

	FFSM2_CONSTEXPR(11)	Long cursor()							  const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

}
}

#if FFSM2_SERIALIZATION_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <Long NBitCapacity>
FFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator == (const StreamBuffer& buffer) const noexcept {
	for (Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return false;

	return true;
}

template <Long NBitCapacity>
FFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator != (const StreamBuffer& buffer) const noexcept {
	for (Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return true;

	return false;
}

template <Long NBitCapacity>
template <Short NBitWidth>
FFSM2_CONSTEXPR(14)
void
BitWriteStreamT<NBitCapacity>::write(const UBitWidth<NBitWidth> item) noexcept {
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

template <Long NBitCapacity>
template <Short NBitWidth>
FFSM2_CONSTEXPR(14)
UBitWidth<NBitWidth>
BitReadStreamT<NBitCapacity>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item item = 0;

	for (Short itemCursor = 0, itemWidth = BIT_WIDTH; itemWidth; ) {
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

}
}

#endif

#endif

namespace ffsm2 {
namespace detail {

template <typename>
struct TypeT {};

template <typename... Ts>
struct TL_ final
	: TypeT<Ts>...
{
	static constexpr Long SIZE = sizeof...(Ts);
};

template <Long N>
struct Const {
	static constexpr Long VALUE = N;
};

template <typename, typename>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TL_<Ts...>> final {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using PrependTypes = typename PrependT<Ts...>::Type;

template <typename, typename>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TL_<Ts1...>, TL_<Ts2...>> final {
	using Type = TL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::Type;

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

template<Long, typename...>
struct FindImpl
	: Const<INVALID_LONG>
{};

template<Long N, typename T, typename TFirst, typename... TRest>
struct FindImpl<N	 , T, TFirst, TRest...>
	:  FindImpl<N + 1, T,		  TRest...>
{};

template<Long N, typename T, typename... Ts>
struct FindImpl<N, T, T, Ts...>
	: Const<N>
{};

template <typename, typename>
struct Find;

template <typename T, typename... Ts>
struct Find<TL_<Ts...>, T> final
	: FindImpl<0, T, Ts...>
{};

}

template <typename TList, typename T>
constexpr Long index   () noexcept	{ return detail::Find<TList, T>::VALUE;					}

template <typename TList, typename T>
constexpr bool contains() noexcept	{ return detail::Find<TList, T>::VALUE != INVALID_LONG;	}

}

namespace ffsm2 {
namespace detail {

template <typename T>
FFSM2_CONSTEXPR(11)
T
filler()																noexcept	{
	return T{};
}

template <>
FFSM2_CONSTEXPR(11)
Short
filler<Short>()															noexcept	{
	return INVALID_SHORT;
}

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
	FFSM2_CONSTEXPR(14)	StaticArrayT(const Item filler)					noexcept	{ fill(filler);						}

	template <typename N>
	FFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)			noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)	  const noexcept;

	FFSM2_CONSTEXPR(11)	Index count()							  const noexcept	{ return CAPACITY;					}

	FFSM2_CONSTEXPR(14)	void fill(const Item filler)					noexcept;
	FFSM2_CONSTEXPR(14)	void clear()									noexcept	{ fill(filler<Item>());				}
	FFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	FFSM2_CONSTEXPR(14)	 Iterator  begin()								noexcept	{ return  Iterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator  begin()						  const noexcept	{ return CIterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator cbegin()						  const noexcept	{ return CIterator(*this, first());	}

	FFSM2_CONSTEXPR(14)	 Iterator	 end()								noexcept	{ return  Iterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	 end()						  const noexcept	{ return CIterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	cend()						  const noexcept	{ return CIterator(*this, limit());	}

private:
	FFSM2_CONSTEXPR(11)	Index first()							  const noexcept	{ return 0;							}
	FFSM2_CONSTEXPR(11)	Index  next(const Index index)			  const noexcept	{ return index + 1;					}
	FFSM2_CONSTEXPR(11)	Index limit()							  const noexcept	{ return CAPACITY;					}

private:
	Item _items[CAPACITY] {};
};

template <typename T>
struct StaticArrayT<T, 0> final {
	using Item		= T;

	FFSM2_CONSTEXPR(11)	StaticArrayT() = default;
	FFSM2_CONSTEXPR(11)	StaticArrayT(const Item)						noexcept	{}
};

template <typename T, Long NCapacity>
class DynamicArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      DynamicArrayT>;
	using CIterator	= IteratorT<const DynamicArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(const TArgs &... args)				noexcept;

	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(		TArgs&&... args)				noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)		  const noexcept;

	FFSM2_CONSTEXPR(11)	Index  count()								  const noexcept	{ return _count;					}

	FFSM2_CONSTEXPR(14)	 void clear()										noexcept	{ _count = 0;						}
	FFSM2_CONSTEXPR(11)	 bool empty()								  const noexcept	{ return _count == 0;				}

	FFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (const Item & item)		noexcept;
	FFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (	  Item&& item)		noexcept;

	template <Long N>
	FFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (const DynamicArrayT<Item, N>& other)	noexcept;

	FFSM2_CONSTEXPR(14)	 Iterator  begin()									noexcept	{ return  Iterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator  begin()							  const noexcept	{ return CIterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator cbegin()							  const noexcept	{ return CIterator(*this, first());	}

	FFSM2_CONSTEXPR(14)	 Iterator	 end()									noexcept	{ return  Iterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	 end()							  const noexcept	{ return CIterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	cend()							  const noexcept	{ return CIterator(*this, limit());	}

private:
	FFSM2_CONSTEXPR(11)	Index first()								  const noexcept	{ return 0;							}
	FFSM2_CONSTEXPR(11)	Index next(const Index index)				  const noexcept	{ return index + 1;					}
	FFSM2_CONSTEXPR(11)	Index limit()								  const noexcept	{ return _count;					}

private:
	Index _count = 0;
	Item _items[CAPACITY] {};
};

template <typename T>
class DynamicArrayT<T, 0> final {
public:
	using Item	= T;
	using Index	= UCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC_>::operator[] (const N index) noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
const T&
StaticArrayT<T, NC_>::operator[] (const N index) const noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC_>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC_>::empty() const noexcept {
	for (const Item& item : _items)
		if (item != filler<Item>())
			return false;

	return true;
}

template <typename T, Long NC_>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Index
DynamicArrayT<T, NC_>::emplace(const TArgs&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{args...};

	return _count++;
}

template <typename T, Long NC_>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Index
DynamicArrayT<T, NC_>::emplace(TArgs&&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{forward<TArgs>(args)...};

	return _count++;
}

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Item&
DynamicArrayT<T, NC_>::operator[] (const N index) noexcept {
	FFSM2_ASSERT(0 <= index && index < _count);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
const typename DynamicArrayT<T, NC_>::Item&
DynamicArrayT<T, NC_>::operator[] (const N index) const noexcept {
	FFSM2_ASSERT(0 <= index && index < _count);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (const Item& item) noexcept {
	emplace(item);

	return *this;
}

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (Item&& item) noexcept {
	emplace(move(item));

	return *this;
}

template <typename T, Long NC_>
template <Long N>
FFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (const DynamicArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		emplace(item);

	return *this;
}

}
}

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <unsigned NCapacity>
class BitArrayT final {
public:
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, 8);

	using BitArray	= BitArrayT<CAPACITY>;

public:
	FFSM2_CONSTEXPR(14)	BitArrayT()										noexcept	{ clear();	}

	FFSM2_CONSTEXPR(14)	void set()										noexcept;

	FFSM2_CONSTEXPR(14)	void clear()									noexcept;

	FFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	bool get  (const TIndex index)			  const noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	void set  (const TIndex index)					noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	void clear(const TIndex index)					noexcept;

	FFSM2_CONSTEXPR(14)	bool operator &  (const BitArray& other)  const noexcept;

	FFSM2_CONSTEXPR(14)	void operator &= (const BitArray& other)		noexcept;

private:
	uint8_t _storage[UNIT_COUNT] {};
};

template <>
class BitArrayT<0> final {
public:
	FFSM2_CONSTEXPR(14)	void clear()									noexcept	{}

	FFSM2_CONSTEXPR(11)	bool empty()							  const noexcept	{ return true;	}
};

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::set() noexcept {
	for (uint8_t& unit : _storage)
		unit = UINT8_MAX;
}

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NC_>::empty() const noexcept {
	for (const uint8_t& unit : _storage)
		if (unit != uint8_t{0})
			return false;

	return true;
}

template <unsigned NC_>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NC_>::get(const TIndex index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

template <unsigned NC_>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::set(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

template <unsigned NC_>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::clear(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NC_>::operator & (const BitArray& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if ((_storage[i] & other._storage[i]) == 0)
			return false;

	return true;
}

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::operator &= (const BitArray& other) noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		_storage[i] &= other._storage[i];
}

}
}

#endif

namespace ffsm2 {

enum class Method : uint8_t {
	NONE,

	ENTRY_GUARD,
	ENTER,
	REENTER,
	PRE_UPDATE,
	UPDATE,
	POST_UPDATE,
	PRE_REACT,
	REACT,
	QUERY,
	POST_REACT,
	EXIT_GUARD,
	EXIT,

//#if FFSM2_PLANS_AVAILABLE()
	PLAN_SUCCEEDED,
	PLAN_FAILED,
//#endif

	COUNT
};

#if FFSM2_PLANS_AVAILABLE()

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

static
FFSM2_CONSTEXPR(14)
const char*
methodName(const Method method)											noexcept	{
	switch (method) {
	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::REENTER:		 return "reenter";
	case Method::PRE_UPDATE:	 return "preUpdate";
	case Method::UPDATE:		 return "update";
	case Method::POST_UPDATE:	 return "postUpdate";
	case Method::PRE_REACT:		 return "preReact";
	case Method::REACT:			 return "react";
	case Method::POST_REACT:	 return "postReact";
	case Method::QUERY:			 return "query";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";

//#if FFSM2_PLANS_AVAILABLE()
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
//#endif

	default:
		FFSM2_BREAK();
		return nullptr;
	}
}

namespace detail {

#pragma pack(push, 1)

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct TransitionBase {
	FFSM2_CONSTEXPR(11)
	TransitionBase()													noexcept = default;

	FFSM2_CONSTEXPR(11)
	TransitionBase(const StateID destination_)							noexcept
		: destination{destination_}
	{}

	FFSM2_CONSTEXPR(11)
	TransitionBase(const StateID origin_,
				   const StateID destination_)							noexcept
		: origin	 {origin_}
		, destination{destination_}
	{}

	FFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionBase& other)					  const noexcept	{
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	FFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionBase& other)					  const noexcept	{
		return origin	   != other.origin ||
			   destination != other.destination ||
			   method	   != other.method;
	}

	FFSM2_CONSTEXPR(11)
	explicit
	operator bool()												  const noexcept	{
		return destination != INVALID_STATE_ID;
	}

	FFSM2_CONSTEXPR(14)
	void
	clear()																noexcept	{
		destination	= INVALID_STATE_ID;
	}

	StateID	origin		= INVALID_STATE_ID;
	StateID	destination	= INVALID_STATE_ID;
	Method	method		= Method::NONE;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

template <typename TPayload>
struct TransitionT final
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];

	using TransitionBase::TransitionBase;

	FFSM2_CONSTEXPR(14)
	TransitionT()														noexcept	{
		new (&storage) Payload{};
	}

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination_,
				const Payload& payload)									noexcept
		: TransitionBase{destination_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin_,
				const StateID destination_,
				const Payload& payload)									noexcept
		: TransitionBase{origin_, destination_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	FFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload == other.payload);
	}

	FFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator != (other) ||
			   (payloadSet != other.payloadSet);
		//	   (payloadSet |= other.payloadSet || payload != other.payload);
	}

	FFSM2_CONSTEXPR(11)
	const Payload*
	payload()													  const noexcept	{
		return payloadSet ?
			reinterpret_cast<const Payload*>(&storage) : nullptr;
	}

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Payload) Storage storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	bool payloadSet = false;
};

template <>
struct TransitionT<void> final
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

}

struct Request final {
	Short index;
};

}

namespace ffsm2 {

struct FFSM2_EMPTY_BASES EmptyContext {};

#if FFSM2_LOG_INTERFACE_AVAILABLE()

template <
	FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
  , typename TContext = EmptyContext
>
struct LoggerInterfaceT {
	using Context		 = TContext;

	using Method		 = ::ffsm2::Method;
	using StateID		 = ::ffsm2::StateID;

#if FFSM2_PLANS_AVAILABLE()
	using StatusEvent	 = ::ffsm2::StatusEvent;
#endif

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordMethod(const Context& FFSM2_UNUSED(context),
				 const StateID FFSM2_UNUSED(origin),
				 const Method FFSM2_UNUSED(method))						noexcept
	{}

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTransition(const Context& FFSM2_UNUSED(context),
					 const StateID FFSM2_UNUSED(origin),
					 const StateID FFSM2_UNUSED(target))				noexcept
	{}

#if FFSM2_PLANS_AVAILABLE()

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTaskStatus(const Context& FFSM2_UNUSED(context),
					 const StateID FFSM2_UNUSED(origin),
					 const StatusEvent FFSM2_UNUSED(event))				noexcept
	{}

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordPlanStatus(const Context& FFSM2_UNUSED(context),
					 const StatusEvent FFSM2_UNUSED(event))				noexcept
	{}

#endif

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordCancelledPending(const Context& FFSM2_UNUSED(context),
						   const StateID FFSM2_UNUSED(origin))			noexcept
	{}

};

#else

template <
	FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
  , typename TContext = EmptyContext
>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

#pragma pack(push, 1)

struct TaskBase {
	static_assert(sizeof(Long) == sizeof(StateID), "");

	FFSM2_CONSTEXPR(11)	TaskBase()										noexcept	{}

	FFSM2_CONSTEXPR(11)	TaskBase(const StateID origin_,
								 const StateID destination_)			noexcept
		: origin{origin_}
		, destination{destination_}
	{}

	FFSM2_CONSTEXPR(11) bool cyclic()							  const noexcept	{ return origin == destination;	}

	union {
		StateID origin		= INVALID_STATE_ID;
		Long prev;
	};

	union {
		StateID destination	= INVALID_STATE_ID;
		Long next;
	};
};

FFSM2_CONSTEXPR(11)
bool
operator == (const TaskBase& lhs,
			 const TaskBase& rhs)										noexcept
{
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination;
}

template <typename TPayload>
struct TaskT final
	: TaskBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];

	using TaskBase::TaskBase;

	FFSM2_CONSTEXPR(14)	TaskT()											noexcept	{
		new (&storage) Payload{};
	}

	FFSM2_CONSTEXPR(14)	TaskT(const StateID origin_,
							  const StateID destination_,
							  const Payload& payload)					noexcept
		: TaskBase{origin_, destination_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	FFSM2_CONSTEXPR(11)
	const Payload*
	payload()													  const noexcept	{
		return payloadSet ?
			reinterpret_cast<const Payload*>(&storage) : nullptr;
	}

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Payload) Storage storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	bool payloadSet = false;
};

template <>
struct TaskT<void> final
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

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
	FFSM2_CONSTEXPR(14)	 void clear()											noexcept;

	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(TArgs&&... args)							noexcept;

	FFSM2_CONSTEXPR(14)	void remove(const Index i)								noexcept;

	FFSM2_CONSTEXPR(14)		  Item& operator[] (const Index i)					noexcept;
	FFSM2_CONSTEXPR(11)	const Item& operator[] (const Index i)			  const noexcept;

	FFSM2_CONSTEXPR(11)	Index count()									  const noexcept	{ return _count;		}
	FFSM2_CONSTEXPR(11)	bool  empty()									  const noexcept	{ return _count == 0;	}

private:
	FFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)  const noexcept);

private:
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last		  = 0;
	Index _count	  = 0;
	Item _items[CAPACITY] {};
};

template <typename TItem>
class TaskListT<TItem, 0> {};

}
}

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TP_, Long NC_>
FFSM2_CONSTEXPR(14)
void
TaskListT<TP_, NC_>::clear() noexcept {
	_vacantHead	= 0;
	_vacantTail	= 0;
	_last		= 0;
	_count		= 0;
}

template <typename TP_, Long NC_>
template <typename... TA_>
FFSM2_CONSTEXPR(14)
Long
TaskListT<TP_, NC_>::emplace(TA_&&... args) noexcept {
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

		new (&item) Item{forward<TA_>(args)...};
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

template <typename TP_, Long NC_>
FFSM2_CONSTEXPR(14)
void
TaskListT<TP_, NC_>::remove(const Index i) noexcept {
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

template <typename TP_, Long NC_>
FFSM2_CONSTEXPR(14)
typename TaskListT<TP_, NC_>::Item&
TaskListT<TP_, NC_>::operator[] (const Index i) noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

template <typename TP_, Long NC_>
FFSM2_CONSTEXPR(11)
const typename TaskListT<TP_, NC_>::Item&
TaskListT<TP_, NC_>::operator[] (const Index i) const noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

#if FFSM2_ASSERT_AVAILABLE()

template <typename TP_, Long NC_>
void
TaskListT<TP_, NC_>::verifyStructure(const Index occupied) const noexcept {
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

}
}

#endif

#endif

namespace ffsm2 {
namespace detail {

template <
	typename
  , typename
  FFSM2_IF_SERIALIZATION(, Long)
  FFSM2_IF_PLANS(, Long)
  , typename
>
struct ArgsT;

struct Registry final {
	FFSM2_CONSTEXPR(11)	bool isActive				()						  const noexcept	{ return active != INVALID_SHORT;								}

	FFSM2_CONSTEXPR(14)	bool isActive				(const StateID stateId)	  const noexcept	{
		return stateId == 0 ?
			active != INVALID_SHORT :
			active == stateId;
	}

	FFSM2_CONSTEXPR(14)	void clearRequests			()								noexcept	{		 requested  = INVALID_SHORT;							}
	FFSM2_CONSTEXPR(14)	void clear					()								noexcept	{		 requested  = INVALID_SHORT;   active  = INVALID_SHORT;	}

	FFSM2_CONSTEXPR(11)	bool empty					()						  const noexcept	{ return requested == INVALID_SHORT && active == INVALID_SHORT;	}

	Short requested	= INVALID_SHORT;
	Short active	= INVALID_SHORT;
};

}
}

namespace ffsm2 {
namespace detail {

#pragma pack(push, 1)

struct TaskStatus final {
	enum Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	FFSM2_CONSTEXPR(11)	TaskStatus(const Result result_ = Result::NONE)	noexcept;

	FFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	FFSM2_CONSTEXPR(14)	void clear()									noexcept;
};

#pragma pack(pop)

FFSM2_CONSTEXPR(14) TaskStatus  operator |  (TaskStatus& l, const TaskStatus r)	noexcept;
FFSM2_CONSTEXPR(14) TaskStatus& operator |= (TaskStatus& l, const TaskStatus r)	noexcept;

#if FFSM2_PLANS_AVAILABLE()

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;

	FFSM2_CONSTEXPR(14) void clear()									noexcept	{ first = INVALID_LONG; last = INVALID_LONG;	}
};

template <
	typename
  , typename
  FFSM2_IF_SERIALIZATION(, Long)
  , Long
  , typename
>
struct ArgsT;

template <typename>
struct PlanDataT;

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
  , typename TPayload
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 FFSM2_IF_SERIALIZATION(, NSerialBits)
			 , NTaskCapacity
			 , TPayload
		   >
	   > final
{
	using StateList			= TStateList;
	using Payload			= TPayload;

	static constexpr Long STATE_COUNT	= StateList ::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <Payload>;
	using Tasks				= TaskListT   <Payload,  TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads			= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBits			= BitArrayT   <				STATE_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;
	TaskStatus headStatus;
	TaskStatus subStatus;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	FFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	FFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()							  const noexcept;
#endif
};

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 FFSM2_IF_SERIALIZATION(, NSerialBits)
			 , NTaskCapacity
			 , void
		   >
	   > final
{
	using StateList			= TStateList;

	static constexpr Long  STATE_COUNT	= StateList ::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <void>;
	using Tasks				= TaskListT	  <void,	 TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBits			= BitArrayT	  <				STATE_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;
	TaskStatus headStatus;
	TaskStatus subStatus;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	FFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	FFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()							  const noexcept;
#endif
};

#endif

}
}

namespace ffsm2 {
namespace detail {

FFSM2_CONSTEXPR(11)
TaskStatus::TaskStatus(const Result result_) noexcept
	: result{result_}
{}

FFSM2_CONSTEXPR(11)
TaskStatus::operator bool() const noexcept {
	return result != Result::NONE;
}

FFSM2_CONSTEXPR(14)
void
TaskStatus::clear() noexcept {
	result = Result::NONE;
}

FFSM2_CONSTEXPR(14)
TaskStatus
operator | (TaskStatus& lhs,
			const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return TaskStatus{result};
}

FFSM2_CONSTEXPR(14)
TaskStatus&
operator |= (TaskStatus& lhs,
			 const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
										  lhs.result : rhs.result;

	lhs = TaskStatus{result};

	return lhs;
}

#if FFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clearRegionStatuses() noexcept {
	headStatus.clear();
	 subStatus.clear();
}

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clear() noexcept {
	tasks		  .clear();
	taskLinks	  .clear();
	taskPayloads  .clear();
	payloadExists .clear();

	tasksBounds	  .clear();
	tasksSuccesses.clear();
	tasksFailures .clear();
	planExists	 = false;

	clearRegionStatuses();
}

#if FFSM2_ASSERT_AVAILABLE()

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyPlan() const noexcept	{
	Long length = 0;
	const Bounds& bounds = tasksBounds;

	if (bounds.first != INVALID_LONG) {
		FFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (Long slow = bounds.first, fast = slow; ; ) {
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

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clearRegionStatuses() noexcept {
	headStatus.clear();
	subStatus .clear();
}

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clear() noexcept {
	tasks		  .clear();
	taskLinks	  .clear();

	tasksBounds	  .clear();
	tasksSuccesses.clear();
	tasksFailures .clear();
	planExists	 = false;

	clearRegionStatuses();
}

#if FFSM2_ASSERT_AVAILABLE()

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyPlan() const noexcept	{
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
#endif

}
}

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TArgs>
class CPlanT {
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	struct Iterator final {
		FFSM2_CONSTEXPR(14)	Iterator(const CPlanT& plan)				noexcept;

		FFSM2_CONSTEXPR(11)	explicit operator bool()			  const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		FFSM2_CONSTEXPR(11)	const Task& operator  *()			  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()			  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

private:
	FFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData)				noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds}
	{}

	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()								noexcept	{ return index<StateList , TState>();		}

public:
	FFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_CONSTEXPR(14)	Iterator begin()								noexcept	{ return Iterator{*this};					}

	/// @brief Iteration terminator
	/// @return Dummy Iterator
	FFSM2_CONSTEXPR(14)	Iterator end  ()								noexcept	{ return Iterator{*this};					}

	/// @brief First task
	/// @return First task
	FFSM2_CONSTEXPR(14) const Task& first()						  const noexcept;

	/// @brief Last task
	/// @return Last task
	FFSM2_CONSTEXPR(14) const Task&  last()						  const noexcept;

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
CPlanT<TArgs>::Iterator::Iterator(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CPlanT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
CPlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
CPlanT<TArgs>::Iterator::next() const noexcept {
	if (_curr < CPlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CPlanT<TArgs>::operator bool() const noexcept {
	FFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
const typename CPlanT<TArgs>::Task&
CPlanT<TArgs>::first() const noexcept {
	FFSM2_ASSERT(_bounds.first < TASK_CAPACITY);

	return _planData.tasks[_bounds.first];
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
const typename CPlanT<TArgs>::Task&
CPlanT<TArgs>::last() const noexcept {
	FFSM2_ASSERT(_bounds.last < TASK_CAPACITY);

	return _planData.tasks[_bounds.last];
}

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TArgs>
class PlanT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long STATE_COUNT	= StateList::SIZE;
	static constexpr Long TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	struct CIterator final {
		FFSM2_CONSTEXPR(14)	CIterator(const PlanT& plan)				noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		FFSM2_CONSTEXPR(14)	const Task& operator  *()			  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()			  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		const PlanT& _plan;
		Long _curr;
		Long _next;
	};

	struct Iterator final {
		FFSM2_CONSTEXPR(14)	Iterator(PlanT& plan)						noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		FFSM2_CONSTEXPR(14)	Task& operator  *()							noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(14)	Task* operator ->()							noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	void remove()								noexcept	{ _plan.remove(_curr);						}

		FFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		PlanT& _plan;
		Long _curr;
		Long _next;
	};

protected:
	FFSM2_CONSTEXPR(11)	PlanT(PlanData& planData)						noexcept;

	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()								noexcept	{ return		index<StateList , TState>();		}

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination)			noexcept;

	FFSM2_CONSTEXPR(14)	bool linkTask(const Long index)					noexcept;

	FFSM2_CONSTEXPR(14)	void clearTasks()								noexcept;

public:
	FFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_CONSTEXPR(14)	void clear()									noexcept;

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool change(const StateID origin,
									const StateID destination)			noexcept	{ return append(origin			  , destination)			;	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool change(const StateID destination)			noexcept	{ return change(stateId<TOrigin>(), destination)			;	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool change()													noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	FFSM2_CONSTEXPR(14)	 Iterator begin()								noexcept	{ return  Iterator{*this};	}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_CONSTEXPR(11)	CIterator begin()						  const noexcept	{ return CIterator{*this};	}

	/// @brief Iteration terminator
	/// @return Dummy Iterator
	FFSM2_CONSTEXPR(14)	 Iterator end()									noexcept	{ return  Iterator{*this};	}

	/// @brief Iteration terminator
	/// @return Dummy CIterator
	FFSM2_CONSTEXPR(11)	CIterator end()							  const noexcept	{ return CIterator{*this};	}

	/// @brief First task
	/// @return First task
	FFSM2_CONSTEXPR(14)		  Task& first()								noexcept;

	/// @brief First task
	/// @return First task
	FFSM2_CONSTEXPR(14) const Task& first()						  const noexcept;

	/// @brief Last task
	/// @return Last task
	FFSM2_CONSTEXPR(14)		  Task&  last()								noexcept;

	/// @brief Last task
	/// @return Last task
	FFSM2_CONSTEXPR(14) const Task&  last()						  const noexcept;

private:
	FFSM2_CONSTEXPR(14)	void remove(const Long task)					noexcept;

protected:
	PlanData& _planData;
	Bounds& _bounds;
};

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::CIterator::CIterator(const PlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::CIterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr  < PlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanT::TASK_CAPACITY;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
PlanT<TArgs>::CIterator::next() const noexcept {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::Iterator::Iterator(PlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanT::TASK_CAPACITY;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
PlanT<TArgs>::Iterator::next() const noexcept {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

template <typename TArgs>
FFSM2_CONSTEXPR(11)
PlanT<TArgs>::PlanT(PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
bool
PlanT<TArgs>::append(const StateID origin,
					 const StateID destination) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists = true;

		return linkTask(_planData.tasks.emplace(origin, destination));
	} else
		return false;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
bool
PlanT<TArgs>::linkTask(const Long index) noexcept	{
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

			TaskLink& lastLink = _planData.taskLinks[_bounds.last];
			FFSM2_ASSERT(lastLink.next == INVALID_LONG);

			lastLink.next  = index;

			TaskLink& currLink = _planData.taskLinks[index];
			FFSM2_ASSERT(currLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clearTasks() noexcept	{
	if (_bounds.first < TaskLinks::CAPACITY) {
		FFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			FFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const TaskLink& link = _planData.taskLinks[index];
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

template <typename TArgs>
FFSM2_CONSTEXPR(11)
PlanT<TArgs>::operator bool() const noexcept {
	FFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clear() noexcept	{
	clearTasks();

	for (StateID i = 0;
		 i < STATE_COUNT;
		 ++i)
	{
		_planData.tasksSuccesses.clear(i);
		_planData.tasksFailures .clear(i);
	}
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::remove(const Long index) noexcept {
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

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TArgs>
class PayloadPlanT;

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
  , typename TPayload
>
class PayloadPlanT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			, NTaskCapacity
			, TPayload
		  >
	  > final
	: public PlanT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , TPayload
				 >
			 >
{
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , TPayload
				 >;

	using Payload		= typename Args::Payload;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

	using PlanBase		= PlanT<Args>;

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	FFSM2_CONSTEXPR(14)	bool  append(const StateID origin,
									 const StateID destination,
									 const Payload& payload)			noexcept;

public:

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool changeWith   (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 ,		payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool changeWith   (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 ,		payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool changeWith	  (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(),		payload );	}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

private:
	using PlanBase::_planData;
};

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
>
class PayloadPlanT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			, NTaskCapacity
			, void
		  >
	  > final
	: public PlanT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , void
				 >
			 >
{
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , void
				 >;

	using PlanBase = PlanT<Args>;

	using PlanBase::PlanBase;
};

}
}

#endif

#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
bool
PayloadPlanT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::append(const StateID origin,
																				  const StateID destination,
																				  const Payload& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, payload));
}

}
}

#endif

namespace ffsm2 {
namespace detail {

template <typename TArgs>
struct CoreT {
	using Context			= typename TArgs::Context;
	using PureContext		= typename TArgs::PureContext;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	Transition previousTransition;
#endif

	FFSM2_CONSTEXPR(11)	explicit CoreT(Context& context_
									 FFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	FFSM2_CONSTEXPR(11)	explicit CoreT(PureContext&& context_
									 FFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	FFSM2_CONSTEXPR(11) CoreT(const CoreT&  other)													noexcept;
	FFSM2_CONSTEXPR(11) CoreT(		CoreT&& other)													noexcept;

	Context context;
	Registry registry;
	Transition request;
	FFSM2_IF_PLANS(PlanData planData);
	FFSM2_IF_LOG_INTERFACE(Logger* logger);
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(Context& context_
				  FFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{context_}
	FFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(PureContext&& context_
				  FFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{move(context_)}
	FFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(const CoreT& other) noexcept
	: context {other.context }
	, registry{other.registry}
	, request {other.request }
	FFSM2_IF_PLANS			   (, planData			 {other.planData		   })
	FFSM2_IF_LOG_INTERFACE	   (, logger			 {other.logger			   })
{}

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(CoreT&& other) noexcept
	: context {move(other.context )}
	, registry{move(other.registry)}
	, request {move(other.request )}
	FFSM2_IF_PLANS			   (, planData			 {move(other.planData			)})
	FFSM2_IF_LOG_INTERFACE	   (, logger			 {move(other.logger				)})
{}

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
class ConstControlT {
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;

	using Core				= CoreT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	struct Origin final {
		FFSM2_CONSTEXPR(14)	Origin(ConstControlT& control_,
								   const StateID stateId_)				noexcept;

		FFSM2_CONSTEXPR(20)	~Origin()									noexcept;

		ConstControlT& control;
		const StateID prevId;
	};

	FFSM2_CONSTEXPR(11)	ConstControlT(const Core& core)					noexcept
		: _core{core}
	{}

public:

#if FFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	FFSM2_CONSTEXPR(11) StateID stateId()						  const noexcept	{ return _originId;							}

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()								noexcept	{ return index<StateList , TState>();		}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(11)	const Context& _()						  const noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(11)	const Context& context()				  const noexcept	{ return _core.context;						}

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11)	const Transition& request()				  const noexcept	{ return _core.request;						}

	/// @brief Check if a state is active
	/// @param `stateId` State identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)  const noexcept	{ return _core.registry.active == stateId_;	}

	/// @brief Check if a state is active
	/// @tparam `TState` State type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive	()						  const noexcept	{ return isActive(stateId<TState>());		}

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()							  const noexcept	{ return CPlan{_core.planData};				}

#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last `update()`, `react()` or `replayTransition()`
	/// @return Array of last transition requests
	/// @see `FFSM2_ENABLE_TRANSITION_HISTORY`
	FFSM2_CONSTEXPR(11)	const Transition& previousTransitions()	  const noexcept	{ return _core.previousTransition;			}

#endif

protected:
	const Core& _core;
	StateID _originId = INVALID_STATE_ID;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Origin::Origin(ConstControlT& control_,
									 const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	FFSM2_ASSERT(stateId_ < StateList::SIZE ||
				 stateId_ == INVALID_STATE_ID); // specific

	control._originId = stateId_;
}

template <typename TArgs>
FFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

}
}

namespace ffsm2 {
namespace detail {

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

	using Core				= CoreT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	struct Origin final {
		FFSM2_CONSTEXPR(14)	 Origin(ControlT& control_,
									const StateID stateId_)					noexcept;

		FFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ControlT& control;
		const StateID prevId;
	};

	FFSM2_CONSTEXPR(11)	ControlT(Core& core)								noexcept
		: _core{core}
	{}

public:

#if FFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	FFSM2_CONSTEXPR(11) StateID stateId()							  const noexcept	{ return _originId;							}

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()									noexcept	{ return index<StateList , TState>();		}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::context()`
	FFSM2_CONSTEXPR(14)		  Context& _()									noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::context()`
	FFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::_()`
	FFSM2_CONSTEXPR(14)		  Context& context()							noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::_()`
	FFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;						}

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11)	const Transition& request()					  const noexcept	{ return _core.request;						}

	/// @brief Check if a state is active
	/// @param `stateId` State identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);	}

	/// @brief Check if a state is active
	/// @tparam `TState` State type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());		}

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.planData};				}

#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last `update()`, `react()` or `replayTransition()`
	/// @return Array of last transition requests
	/// @see `FFSM2_ENABLE_TRANSITION_HISTORY`
	FFSM2_CONSTEXPR(11)	const Transition& previousTransitions()		  const noexcept	{ return _core.previousTransition;			}

#endif

protected:
	Core& _core;
	StateID _originId = INVALID_STATE_ID;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	FFSM2_ASSERT(stateId_ < StateList::SIZE ||
				 stateId_ == INVALID_STATE_ID); // specific

	control._originId = stateId_;
}

template <typename TArgs>
FFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

}
}

namespace ffsm2 {
namespace detail {

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
	using Control			= ControlT<TArgs>;

	using typename Control::StateList;

	using typename Control::Core;

	using Transition		= typename Core::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
#endif

	struct Region {
		FFSM2_CONSTEXPR(14)	Region(PlanControlT& control)					noexcept;

		FFSM2_CONSTEXPR(20)	~Region()										noexcept;

		PlanControlT& control;
	};

	FFSM2_CONSTEXPR(11)	PlanControlT(Core& core,
									 const Transition& currentTransition)	noexcept
		: Control{core}
		, _currentTransition{currentTransition}
	{}

	FFSM2_CONSTEXPR(14)	void   setRegion()									noexcept;
	FFSM2_CONSTEXPR(14)	void resetRegion()									noexcept;

public:

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::CPlan;

	using Plan			= PayloadPlanT<TArgs>;

	/// @brief Access plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	 Plan plan()										noexcept	{ return  Plan{_core.planData};		}

	/// @brief Access read-only plan
	/// @return Read-only plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.planData};		}

#endif

	/// @brief Get current transition request
	/// @return Current transition request
	FFSM2_CONSTEXPR(11)	const Transition& currentTransition()		  const noexcept	{ return _currentTransition;	}

protected:
	using Control::_core;

	const Transition& _currentTransition;

	TaskStatus _taskStatus;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanControlT<TArgs>::Region::Region(PlanControlT& control_) noexcept
	: control  {control_}
{
	control.setRegion();
}

template <typename TArgs>
FFSM2_CONSTEXPR(20)
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion();
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::setRegion() noexcept
{
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::resetRegion() noexcept
{
	_taskStatus.clear();
}

}
}

namespace ffsm2 {
namespace detail {

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

	struct Lock final {
		FFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)			noexcept;
		FFSM2_CONSTEXPR(20)	~Lock()										noexcept;

		FullControlBaseT* const control;
	};

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	/// @brief Transition into a state
	/// @param `stateId` State identifier
	FFSM2_CONSTEXPR(14)	void changeTo(const StateID stateId_)			noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` State type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo()									noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	void succeed()								noexcept	{ succeed  (_originId							   );	}

	/// @brief Succeed a plan task for a state
	/// @param `stateId` State identifier
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)		noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void succeed()								noexcept	{ succeed  (PlanControl::template stateId<TState>());	}

	/// @brief Fail a plan task for the current state
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	void fail	()								noexcept	{ fail	   (_originId							   );	}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)		noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void fail	()								noexcept	{ fail	   (PlanControl::template stateId<TState>());	}

#endif

protected:
	using PlanControl::_core;
	using PlanControl::_originId;

	using PlanControl::_taskStatus;

	bool _locked = false;
};

template <typename TArgs>
class FullControlT;

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
class FullControlT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			FFSM2_IF_PLANS(, NTaskCapacity)
			, TPayload
		  >
	  >
	: public FullControlBaseT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , TPayload
				 >
			 >
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , TPayload
				 >;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	using FullControlBase::FullControlBase;

#if FFSM2_PLANS_AVAILABLE()

	template <typename TState>
	FFSM2_CONSTEXPR(14)	void updatePlan(TState& headState,
										const TaskStatus subStatus)		noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

	/// @brief Transition into a state
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)			noexcept	{ changeWith(FullControlBase::template stateId<TState>(),	   payload );	}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

protected:
	using FullControlBase::_core;
	using FullControlBase::_originId;

	using FullControlBase::_taskStatus;

	using FullControlBase::_locked;
};

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  FFSM2_IF_PLANS(, Long NTaskCapacity)
>
class FullControlT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			FFSM2_IF_PLANS(, NTaskCapacity)
			, void
		  >
	  >
	: public FullControlBaseT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , void
				 >
			 >
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , void
				 >;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	using FullControlBase::FullControlBase;

#if FFSM2_PLANS_AVAILABLE()

	template <typename TState>
	FFSM2_CONSTEXPR(14)	void updatePlan(TState& headState,
										const TaskStatus subStatus)		noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	FFSM2_IF_PLANS(using FullControlBase::_core);

	using FullControlBase::_taskStatus;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

template <typename TArgs>
FFSM2_CONSTEXPR(20)
FullControlBaseT<TArgs>::Lock::~Lock() noexcept	{
	if (control)
		control->_locked = false;
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.request = Transition{_originId, stateId_};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId_);
	}
}

#if FFSM2_PLANS_AVAILABLE()

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::SUCCESS;

	_core.planData.tasksSuccesses.set(stateId_);

	FFSM2_LOG_TASK_STATUS(context(), stateId_, StatusEvent::SUCCEEDED);
}

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::FAILURE;

	_core.planData.tasksFailures.set(stateId_);

	FFSM2_LOG_TASK_STATUS(context(), stateId_, StatusEvent::FAILED);
}

#endif

#if FFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::updatePlan(TState& headState,
																					  const TaskStatus subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits successesToClear;
			successesToClear.set();

			for (auto it = p.begin();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, it->origin}; // SPECIFIC

					if (const Payload* const payload = it->payload())
						changeWith(it->destination, *it->payload());
					else
						changeTo  (it->destination);

					if (it->cyclic())
						_core.planData.tasksSuccesses.clear(it->origin); // SPECIFIC
					else
						successesToClear.clear(it->origin);

					it.remove();
				}
			}

			_core.planData.tasksSuccesses &= successesToClear;
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();
		}
	}
}

#endif

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_) FFSM2_IF_PLANS(, Long NTC_), typename TTP_>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_) FFSM2_IF_PLANS(, NTC_), TTP_>>::changeWith(const StateID  stateId_,
																									   const Payload& payload) noexcept
{
	if (!_locked) {
		_core.request = Transition{_originId, stateId_, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId_);
	}
}

#if FFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::updatePlan(TState& headState,
																					  const TaskStatus subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits successesToClear;
			successesToClear.set();

			for (auto it = p.begin();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, it->origin}; // SPECIFIC

					changeTo(it->destination);

					if (it->cyclic())
						_core.planData.tasksSuccesses.clear(it->origin); // SPECIFIC
					else
						successesToClear.clear(it->origin);

					it.remove();
				}
			}

			_core.planData.tasksSuccesses &= successesToClear;
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();
		}
	}
}

#endif

}
}

namespace ffsm2 {
namespace detail {

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

	using typename FullControl::Core;
	using typename FullControl::Context;

	using typename FullControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	FFSM2_CONSTEXPR(11)	GuardControlT(Core& core,
									  const Transition& currentTransition,
									  const Transition& pendingTransition) noexcept
		: FullControl{core, currentTransition}
		, _pendingTransition{pendingTransition}
	{}

public:
	using FullControl::context;

	/// @brief Get pending transition request
	/// @return Pending transition request
	FFSM2_CONSTEXPR(11)	const Transition& pendingTransition()	  const noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition request
	///   (can be used to substitute a transition into the current state with a different one)
	FFSM2_CONSTEXPR(14)	void cancelPendingTransition()					noexcept;

private:
	using FullControl::_core;
	using FullControl::_originId;

	const Transition& _pendingTransition;

	bool _cancelled = false;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
GuardControlT<TArgs>::cancelPendingTransition() noexcept {
	_cancelled = true;

	FFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

}
}

namespace ffsm2 {
namespace detail {

template <typename TArgs>
class B_ {
	template <typename...>
	friend struct A_;

protected:
	using Context		= typename TArgs::Context;

	using StateList		= typename TArgs::StateList;

	using ConstControl	= ConstControlT<TArgs>;
	using Control		= ControlT	   <TArgs>;
	using PlanControl	= PlanControlT <TArgs>;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= PayloadPlanT <TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:

	FFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	query		  (		 TEvent&		,
											   ConstControl&		)	  const noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)			noexcept	{}

	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}
};

}
}

namespace ffsm2 {
namespace detail {

template <typename...>
struct A_;

template <typename TArgs>
using EmptyT = A_<B_<TArgs>>;

template <typename TFirst, typename... TRest>
struct FFSM2_EMPTY_BASES A_<TFirst, TRest...>
	: TFirst
	, A_<TRest...>
{
	using First		= TFirst;
	using typename First::Context;

	using typename First::StateList;

	using typename First::ConstControl;
	using typename First::Control;
	using typename First::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename First::Plan;
#endif

	using typename First::FullControl;
	using typename First::GuardControl;

	using First::stateId;

	using Rest		= A_<TRest...>;

	FFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	wideQuery	  (		 TEvent& event	,
											   ConstControl& control)	  const noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)			noexcept;

};

}
}

namespace ffsm2 {
namespace detail {

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideEntryGuard(GuardControl& control) noexcept {
	First::	   entryGuard(control);
	Rest ::wideEntryGuard(control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideEnter(PlanControl& control) noexcept {
	First::	   enter(control);
	Rest ::wideEnter(control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideReenter(PlanControl& control) noexcept {
	First::	   reenter(control);
	Rest ::wideReenter(control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePreUpdate(FullControl& control) noexcept {
	First::	   preUpdate(control);
	Rest ::widePreUpdate(control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideUpdate(FullControl& control) noexcept {
	First::	   update(control);
	Rest ::wideUpdate(control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePostUpdate(FullControl& control) noexcept {
	Rest ::widePostUpdate(control);
	First::	   postUpdate(control);
}

template <typename TF_, typename... TR_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePreReact(const TEvent& event,
							  FullControl& control) noexcept
{
	First::	   preReact(event, control);
	Rest ::widePreReact(event, control);
}

template <typename TF_, typename... TR_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideReact(const TEvent& event,
						   FullControl& control) noexcept
{
	First::	   react(event, control);
	Rest ::wideReact(event, control);
}

template <typename TF_, typename... TR_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePostReact(const TEvent& event,
							   FullControl& control) noexcept
{
	Rest ::widePostReact(event, control);
	First::	   postReact(event, control);
}

template <typename TF_, typename... TR_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideQuery(TEvent& event,
						   ConstControl& control) const noexcept
{
	First::	   query(event, control);
	Rest ::wideQuery(event, control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideExitGuard(GuardControl& control) noexcept {
	Rest ::wideExitGuard(control);
	First::	   exitGuard(control);
}

template <typename TF_, typename... TR_>
FFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideExit(PlanControl& control) noexcept {
	Rest ::wideExit(control);
	First::	   exit(control);
}

}
}

namespace ffsm2 {
namespace detail {

template <typename TFirst>
struct A_<TFirst>
	: TFirst
{
	using First		= TFirst;
	using typename First::Context;

	using typename First::StateList;

	using typename First::ConstControl;
	using typename First::Control;
	using typename First::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename First::Plan;
#endif

	using typename First::FullControl;
	using typename First::GuardControl;

	using First::stateId;

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	FFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	query		  (		 TEvent&		,
											   ConstControl&		)	  const noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)			noexcept	{}

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void	planSucceeded ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	planFailed	  ( FullControl&		)			noexcept	{}
#endif

	FFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	wideQuery	  (		 TEvent& event	,
											   ConstControl& control)	  const noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)			noexcept;

};

}
}

namespace ffsm2 {
namespace detail {

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideEntryGuard(GuardControl& control) noexcept {
	First::	   entryGuard(control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideEnter(PlanControl& control) noexcept {
	First::	   enter(control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideReenter(PlanControl& control) noexcept {
	First::	   reenter(control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::widePreUpdate(FullControl& control) noexcept {
	First::	   preUpdate(control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideUpdate(FullControl& control) noexcept {
	First::	   update(control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::widePostUpdate(FullControl& control) noexcept {
	First::	   postUpdate(control);
}

template <typename TF_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::widePreReact(const TEvent& event,
					  FullControl& control) noexcept
{
	First::	   preReact(event, control);
}

template <typename TF_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideReact(const TEvent& event,
				   FullControl& control) noexcept
{
	First::	   react(event, control);
}

template <typename TF_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::widePostReact(const TEvent& event,
					   FullControl& control) noexcept
{
	First::	   postReact(event, control);
}

template <typename TF_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideQuery(TEvent& event,
				   ConstControl& control) const noexcept
{
	First::	   query(event, control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideExitGuard(GuardControl& control) noexcept {
	First::	   exitGuard(control);
}

template <typename TF_>
FFSM2_CONSTEXPR(14)
void
A_<TF_>::wideExit(PlanControl& control) noexcept {
	First::	   exit(control);
}

}
}

namespace ffsm2 {
namespace detail {

template <
	StateID NStateId
  , typename TArgs
  , typename THead
>
struct S_
	: THead
{
	static constexpr StateID STATE_ID = NStateId;

	using Context		= typename TArgs::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TArgs::Logger;
#endif

	using ConstControl	= ConstControlT<TArgs>;
	using ScopedCOrigin	= typename ConstControl::Origin;

	using Control		= ControlT	   <TArgs>;

	using PlanControl	= PlanControlT <TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Empty			= EmptyT<TArgs>;
	using Head			= THead;

	FFSM2_CONSTEXPR(14)	bool		deepEntryGuard		 (GuardControl&	control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepEnter			 ( PlanControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepReenter			 ( PlanControl& control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	TaskStatus	deepPreUpdate		 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	deepUpdate			 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPostUpdate		 ( FullControl& control						 )			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPreReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepReact			 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPostReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepQuery			 (ConstControl& control,	   TEvent&  event)	  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	TaskStatus	deepUpdatePlans		 ( FullControl& control)								noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		deepExitGuard		 (GuardControl&	control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepExit			 ( PlanControl& control						 )			noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void		wrapPlanSucceeded	 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		wrapPlanFailed		 ( FullControl& control						 )			noexcept;
#endif

	FFSM2_CONSTEXPR(14)	void		deepChangeToRequested(	   Control&								 )			noexcept	{}

#if FFSM2_DEBUG_STATE_TYPE_AVAILABLE() || FFSM2_STRUCTURE_REPORT_AVAILABLE() || FFSM2_LOG_INTERFACE_AVAILABLE()

	FFSM2_IF_TYPEINDEX(const std::type_index TYPE = typeid(Head));

#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	template <typename TReturn, typename THost, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...),
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	template <typename TReturn, typename THost, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...) const,
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...) const,
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

#endif
};

}
}

namespace ffsm2 {
namespace detail {

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
bool
S_<NN_, TA_, TH_>::deepEntryGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideEntryGuard(control);
	Head::	  entryGuard(control);

	return !cancelledBefore && control._cancelled;
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, TH_>::deepEnter(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideEnter(control);
	Head::	  enter(control);
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, TH_>::deepReenter(PlanControl& control) noexcept {
	FFSM2_IF_PLANS(control._core.planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReenter(control);
	Head::	  reenter(control);
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepPreUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::preUpdate,
						   Method::PRE_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreUpdate(control);
	Head::	  preUpdate(control);

	return control._taskStatus;
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideUpdate(control);
	Head::	  update(control);

	return control._taskStatus;
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepPostUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::postUpdate,
						   Method::POST_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::	  postUpdate(control);
	Head::widePostUpdate(control);

	return control._taskStatus;
}

template <StateID NN_, typename TA_, typename TH_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepPreReact(FullControl& control,
							 const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::preReact);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::PRE_REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReact(event, control);
	(this->*method)	  (event, control);

	return control._taskStatus;
}

template <StateID NN_, typename TA_, typename TH_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepReact(FullControl& control,
						  const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReact(event, control);
	(this->*method)(event, control);

	return control._taskStatus;
}

template <StateID NN_, typename TA_, typename TH_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepPostReact(FullControl& control,
							  const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::postReact);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::POST_REACT);

	ScopedOrigin origin{control, STATE_ID};

	(this->*method)	   (event, control);
	Head::widePostReact(event, control);

	return control._taskStatus;
}

template <StateID NN_, typename TA_, typename TH_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, TH_>::deepQuery(ConstControl& control,
						  TEvent&  event) const noexcept
{
	auto method = static_cast<void (Head::*)(TEvent&, ConstControl&) const>(&Head::query);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::QUERY);

	ScopedCOrigin origin{control, STATE_ID};

	(this->*method)(event, control);
	Head::wideQuery(event, control);
}

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, TH_>::deepUpdatePlans(FullControl& control) noexcept {
	if (control._core.planData.tasksFailures .get(STATE_ID))
		return TaskStatus{TaskStatus::FAILURE};
	else
	if (control._core.planData.tasksSuccesses.get(STATE_ID))
		return TaskStatus{TaskStatus::SUCCESS};
	else
		return TaskStatus{};
}

#endif

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
bool
S_<NN_, TA_, TH_>::deepExitGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideExitGuard(control);
	Head::	  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, TH_>::deepExit(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from ffsm2::Machine::Instance::State
	Head::	  exit(control);
	Head::wideExit(control);

	FFSM2_IF_PLANS(control._core.planData.clearTaskStatus(STATE_ID));
}

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, TH_>::wrapPlanSucceeded(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planSucceeded(control);
}

template <StateID NN_, typename TA_, typename TH_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, TH_>::wrapPlanFailed(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planFailed(control);
}

#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

}
}

namespace ffsm2 {
namespace detail {

#if FFSM2_DEBUG_STATE_TYPE_AVAILABLE() || FFSM2_STRUCTURE_REPORT_AVAILABLE() || FFSM2_LOG_INTERFACE_AVAILABLE()

struct None {};

#endif

template <
	StateID NStateId
  , typename TArgs
>
struct S_<NStateId, TArgs, EmptyT<TArgs>>
	: EmptyT<TArgs>
{
	static constexpr StateID STATE_ID = NStateId;

	using Context		= typename TArgs::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TArgs::Logger;
#endif

	using ConstControl	= ConstControlT<TArgs>;
	using ScopedCOrigin	= typename ConstControl::Origin;

	using Control		= ControlT	   <TArgs>;

	using PlanControl	= PlanControlT <TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Empty			= EmptyT<TArgs>;
	using Head			= Empty;

	FFSM2_CONSTEXPR(14)	bool		deepEntryGuard		 (GuardControl&	control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepEnter			 ( PlanControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepReenter			 ( PlanControl& control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	TaskStatus	deepPreUpdate		 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	deepUpdate			 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPostUpdate		 ( FullControl& control						 )			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPreReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepReact			 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPostReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepQuery			 (ConstControl& control,	   TEvent&  event)	  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	TaskStatus	deepUpdatePlans		 ( FullControl& control)								noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		deepExitGuard		 (GuardControl&	control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepExit			 ( PlanControl& control						 )			noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void		wrapPlanSucceeded	 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		wrapPlanFailed		 ( FullControl& control						 )			noexcept;
#endif

	FFSM2_CONSTEXPR(14)	void		deepChangeToRequested(	   Control&								 )			noexcept	{}

#if FFSM2_DEBUG_STATE_TYPE_AVAILABLE() || FFSM2_STRUCTURE_REPORT_AVAILABLE() || FFSM2_LOG_INTERFACE_AVAILABLE()

	FFSM2_IF_TYPEINDEX(const std::type_index TYPE = typeid(None));

#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...) const,
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

#endif
};

}
}

namespace ffsm2 {
namespace detail {

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
S_<NN_, TA_, EmptyT<TA_>>::deepEntryGuard(GuardControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::entryGuard,
						   Method::ENTRY_GUARD);

	return false;
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepEnter(PlanControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::enter,
						   Method::ENTER);
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepReenter(PlanControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::reenter,
						   Method::REENTER);
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPreUpdate(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::preUpdate,
						   Method::PRE_UPDATE);

	return TaskStatus{};
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepUpdate(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::update,
						   Method::UPDATE);

	return TaskStatus{};
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPostUpdate(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::postUpdate,
						   Method::POST_UPDATE);

	return TaskStatus{};
}

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPreReact(FullControl& FFSM2_IF_LOG_STATE_METHOD(control),
										const TEvent& FFSM2_UNUSED(event)) noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, FullControl&)>(&Empty::preReact),
						   Method::PRE_REACT);

	return TaskStatus{};
}

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepReact(FullControl& FFSM2_IF_LOG_STATE_METHOD(control),
									 const TEvent& FFSM2_UNUSED(event)) noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, FullControl&)>(&Empty::react),
						   Method::REACT);

	return TaskStatus{};
}

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPostReact(FullControl& FFSM2_IF_LOG_STATE_METHOD(control),
										 const TEvent& FFSM2_UNUSED(event)) noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, FullControl&)>(&Empty::postReact),
						   Method::POST_REACT);

	return TaskStatus{};
}

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepQuery(ConstControl& FFSM2_IF_LOG_STATE_METHOD(control),
									 TEvent& FFSM2_UNUSED(event)) const noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(TEvent&, ConstControl&) const>(&Empty::query),
						   Method::QUERY);
}

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepUpdatePlans(FullControl& control) noexcept {
	if (control._core.planData.tasksFailures .get(STATE_ID))
		return TaskStatus{TaskStatus::FAILURE};
	else
	if (control._core.planData.tasksSuccesses.get(STATE_ID))
		return TaskStatus{TaskStatus::SUCCESS};
	else
		return TaskStatus{};
}

#endif

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
S_<NN_, TA_, EmptyT<TA_>>::deepExitGuard(GuardControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::exitGuard,
						   Method::EXIT_GUARD);

	return false;
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepExit(PlanControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::exit,
						   Method::EXIT);
}

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::wrapPlanSucceeded(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::planSucceeded,
						   Method::PLAN_SUCCEEDED);
}

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::wrapPlanFailed(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::planFailed,
						   Method::PLAN_FAILED);

}

#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

}
}

namespace ffsm2 {
namespace detail {

template <typename>
struct SI_;

template <typename, typename...>
struct CI_;

template <typename>
struct CSI_;

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT	<TH> final {
	using Type = SI_<TH>;
};

template <typename TH, typename... TS>
struct WrapInfoT<CI_<TH, TS...>> final {
	using Type = CI_<TH, TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;

	static constexpr Short WIDTH		  = 1;

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;
};

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TL_<TSubStates...>>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  WIDTH_BITS	  = static_cast<Long>(bitWidth(WIDTH));
	static constexpr Long  ACTIVE_BITS	  = WIDTH_BITS;
#endif
};

template <typename TI, typename... TR>
struct CSI_<TL_<TI, TR...>> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= CSI_<TL_<TR...>>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;
};

template <typename TI>
struct CSI_<TL_<TI>> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;
};

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct ArgsT final {
	using Config		= TConfig;
	using Context		= typename Config::Context;
	using PureContext	= Undecorate<Context>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename Config::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT			= StateList ::SIZE;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short SERIAL_BITS			= NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT	= Config::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY		= NTaskCapacity;
#endif

	using Payload		= TPayload;

#if FFSM2_SERIALIZATION_AVAILABLE()
	using SerialBuffer	= StreamBufferT  <SERIAL_BITS>;
	using WriteStream	= BitWriteStreamT<SERIAL_BITS>;
	using ReadStream	= BitReadStreamT <SERIAL_BITS>;
#endif
};

template <StateID, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <StateID, typename, Short, typename>
struct CS_;

template <typename, typename>
class InstanceT;

template <StateID, typename...>
struct MaterialT_;

template <StateID NN, typename TA, typename TH>
struct MaterialT_  <NN, TA, TH> final {
	using Type = S_<NN, TA, TH>;
};

template <StateID NN, typename TA, 				typename... TS>
struct MaterialT_  <NN, TA, CI_<void	  , TS...>> {
	using Type = C_<	TA,		EmptyT<TA>, TS...>;
};

template <StateID NN, typename TA, typename TH, typename... TS>
struct MaterialT_  <NN, TA, CI_<TH, TS...>> {
	using Type = C_<	TA,		TH, TS...>;
};

template <StateID NN, typename... TS>
using MaterialT = typename MaterialT_<NN, TS...>::Type;

template <
	typename TConfig
  , typename TApex
>
struct RF_ final {
	using Config			= TConfig;
	using Context			= typename Config::Context;
	using Apex				= TApex;

	using StateList			= typename Apex::StateList;

	static constexpr Long  STATE_COUNT			= Apex::STATE_COUNT;

	static constexpr Short SUBSTITUTION_LIMIT	= Config::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY		= Config::TASK_CAPACITY != INVALID_LONG ?
													  Config::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload			= typename Config::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using Task				= typename Config::Task;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
	static constexpr Long  SERIAL_BITS			= 1 + ACTIVE_BITS;
#endif

	using Args				= ArgsT<
								  TConfig
								, StateList
								FFSM2_IF_SERIALIZATION(, SERIAL_BITS)
								FFSM2_IF_PLANS(, TASK_CAPACITY)
								, Payload
							  >;

	using Instance			= InstanceT<Config, Apex>;

	using ConstControl		= ConstControlT<Args>;
	using Control			= ControlT	   <Args>;
	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;

	using State				= EmptyT	   <Args>;

	template <typename... TInjections>
	using StateT			= A_<TInjections...>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename Config::LoggerInterface;
#endif

	template <typename TState>
	static constexpr bool	  contains()		noexcept	{ return					contains<StateList , TState>() ;	}

	template <typename TState>
	static constexpr StateID  stateId()			noexcept	{ return					   index<StateList , TState>() ;	}
};

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

}
}

namespace ffsm2 {
namespace detail {

template <
	StateID NStateId
  , typename TArgs
  , Prong NProng
  , typename... TStates
>
struct FFSM2_EMPTY_BASES CS_<
							 NStateId
						   , TArgs
						   , NProng
						   , TL_<TStates...>
						 >
	:					 LHalfCS<
							 NStateId
						   , TArgs
						   , NProng
						   , TL_<TStates...>
						 >
	,					 RHalfCS<
							 NStateId
						   , TArgs
						   , NProng
						   , TL_<TStates...>
						 >
{
	static_assert(sizeof...(TStates) >= 2, "");

	static constexpr Prong	  PRONG_INDEX = NProng;

	using Args			= TArgs;
	using SubStateList	= TL_<TStates...>;

	using StateList		= typename Args::StateList;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr StateID  INITIAL_ID  = NStateId;
	static constexpr Prong	  L_PRONG	  = PRONG_INDEX;
	static constexpr Prong	  R_PRONG	  = PRONG_INDEX + sizeof...(TStates) / 2;

	using LHalf			= LHalfCS<
							  INITIAL_ID
							, Args
							, PRONG_INDEX
							, SubStateList
						  >;

	using RHalf			= RHalfCS<
							  INITIAL_ID
							, Args
							, PRONG_INDEX
							, SubStateList
						  >;

	FFSM2_CONSTEXPR(14)	bool		wideEntryGuard		 (GuardControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideEnter			 ( PlanControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	void		wideReenter			 ( PlanControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	TaskStatus	widePreUpdate		 ( FullControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	wideUpdate			 ( FullControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	widePostUpdate		 ( FullControl& control,					  const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	widePreReact		 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	wideReact			 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	widePostReact		 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		wideQuery			 (ConstControl& control,	   TEvent& event, const Prong prong)  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	TaskStatus	wideUpdatePlans		 ( FullControl& control,					  const Prong prong)		noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		wideExitGuard		 (GuardControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideExit			 ( PlanControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideChangeToRequested( PlanControl& control,					  const Prong prong)		noexcept;

};

}
}

namespace ffsm2 {
namespace detail {

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideEntryGuard(GuardControl& control,
												const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideEnter(PlanControl& control,
										   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideReenter(PlanControl& control,
											 const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePreUpdate(FullControl& control,
											   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePreUpdate(control, prong) :
		RHalf::widePreUpdate(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideUpdate(FullControl& control,
											const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePostUpdate(FullControl& control,
												const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePostUpdate(control, prong) :
		RHalf::widePostUpdate(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePreReact(FullControl& control,
											  const TEvent& event,
											  const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePreReact(control, event, prong) :
		RHalf::widePreReact(control, event, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideReact(FullControl& control,
										   const TEvent& event,
										   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePostReact(FullControl& control,
											   const TEvent& event,
											   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePostReact(control, event, prong) :
		RHalf::widePostReact(control, event, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideQuery(ConstControl& control,
										   TEvent& event,
										   const Prong prong) const noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideQuery(control, event, prong) :
		RHalf::wideQuery(control, event, prong);
}

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideUpdatePlans(FullControl& control,
												 const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideUpdatePlans(control, prong) :
		RHalf::wideUpdatePlans(control, prong);
}

#endif

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideExitGuard(GuardControl& control,
											   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideExit(PlanControl& control,
										  const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideChangeToRequested(PlanControl& control,
													   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

}
}

namespace ffsm2 {
namespace detail {

template <
	StateID NStateId
  , typename TArgs
  , Prong NProng
  , typename TState
>
struct CS_<
		   NStateId
		 , TArgs
		 , NProng
		 , TL_<TState>
	   >
	:  MaterialT<
		   NStateId
		 , TArgs
		 , TState
	   >
{
	static constexpr Prong	  PRONG_INDEX = NProng;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Single		= MaterialT<
							  NStateId
							, Args
							, TState
						  >;

	FFSM2_CONSTEXPR(14)	bool		wideEntryGuard		 (GuardControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideEnter			 ( PlanControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	void		wideReenter			 ( PlanControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	TaskStatus	widePreUpdate		 ( FullControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	wideUpdate			 ( FullControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	widePostUpdate		 ( FullControl& control,					  const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	widePreReact		 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	wideReact			 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	widePostReact		 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		wideQuery			 (ConstControl& control,	   TEvent& event, const Prong prong)  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	TaskStatus	wideUpdatePlans		 ( FullControl& control,					  const Prong prong)		noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		wideExitGuard		 (GuardControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideExit			 ( PlanControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideChangeToRequested( PlanControl& control,					  const Prong prong)		noexcept;

};

}
}

namespace ffsm2 {
namespace detail {

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<T>>::wideEntryGuard(GuardControl& control,
										   const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepEntryGuard(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideEnter(PlanControl& control,
									  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepEnter(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideReenter(PlanControl& control,
										const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepReenter(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePreUpdate(FullControl& control,
										  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreUpdate(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::wideUpdate(FullControl& control,
									   const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdate(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePostUpdate(FullControl& control,
										   const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostUpdate(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePreReact(FullControl& control,
										 const TEvent& event,
										 const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreReact(control, event);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::wideReact(FullControl& control,
									  const TEvent& event,
									  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReact(control, event);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePostReact(FullControl& control,
										  const TEvent& event,
										  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostReact(control, event);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideQuery(ConstControl& control,
									  TEvent& event,
									  const Prong FFSM2_IF_ASSERT(prong)) const noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepQuery(control, event);
}

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::wideUpdatePlans(FullControl& control,
											const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdatePlans(control);
}

#endif

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<T>>::wideExitGuard(GuardControl& control,
										  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepExitGuard(control);
}

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideExit(PlanControl& control,
									 const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepExit(control);
}

}
}

namespace ffsm2 {
namespace detail {

template <
	typename TArgs
  , typename THead
  , typename... TSubStates
>
struct FFSM2_EMPTY_BASES C_
	: S_<INVALID_STATE_ID, TArgs, THead>
	, CS_<
		  0
		, TArgs
		, 0
		, TL_<TSubStates...>
	  >
{
	using Args			= TArgs;

	using HeadState		= S_<INVALID_STATE_ID, Args, THead>;
	using SubStates		= CS_<0, Args, 0, TL_<TSubStates...>>;

	using StateList		= typename Args::StateList;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT <Args>;
	using ScopedRegion	= typename PlanControl::Region;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= typename PlanControl::Plan;
#endif

	using FullControl	= FullControlT <Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData		= typename Control::PlanData;
#endif

	using Head			= THead;
	using Info			= CI_<Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	FFSM2_CONSTEXPR(11)	static Prong		compoRequested		  (const Registry& registry)	noexcept	{ return registry.requested;	}
	FFSM2_CONSTEXPR(11)	static Prong&		compoRequested		  (		 Registry& registry)	noexcept	{ return registry.requested;	}
#endif

	FFSM2_CONSTEXPR(11)	static Prong&		compoRequested		  (		Control& control)		noexcept	{ return control._core.registry.requested;	}

	FFSM2_CONSTEXPR(11)	static Prong&		compoActive			  (		Control& control)		noexcept	{ return control._core.registry.active;		}
	FFSM2_CONSTEXPR(11)	static Prong		compoActive			  (ConstControl& control)		noexcept	{ return control._core.registry.active;		}

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(11)	static TaskStatus&	headStatus			  (		Control& control)		noexcept	{ return control._core.planData.headStatus;	}
	FFSM2_CONSTEXPR(11)	static TaskStatus&	 subStatus			  (		Control& control)		noexcept	{ return control._core.planData.subStatus;	}
#endif

	FFSM2_CONSTEXPR(14)	bool		deepForwardEntryGuard		  (GuardControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	bool		deepEntryGuard				  (GuardControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepEnter					  ( PlanControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepReenter					  ( PlanControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepPreUpdate				  ( FullControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepUpdate					  ( FullControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepPostUpdate				  ( FullControl& control					 )			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepPreReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepReact					  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepPostReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepQuery					  (ConstControl& control,		TEvent& event)	  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void		deepUpdatePlans				  ( FullControl& control					 )			noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		deepForwardExitGuard		  (GuardControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	bool		deepExitGuard				  (GuardControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepExit					  ( PlanControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepChangeToRequested		  ( PlanControl& control					 )			noexcept;

#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	FFSM2_CONSTEXPR(14)	void		deepSaveActive				  (const Registry& registry, WriteStream& stream) const noexcept;
	FFSM2_CONSTEXPR(14)	void		deepLoadRequested			  (		 Registry& registry,  ReadStream& stream) const noexcept;
#endif

};

}
}

namespace ffsm2 {
namespace detail {

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
C_<TA_, TH_, TS_...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const Prong	 requested	= compoRequested(control);
	FFSM2_ASSERT(requested	< WIDTH);

	FFSM2_ASSERT(compoActive(control) < WIDTH);

	return SubStates::wideEntryGuard(control, requested);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
C_<TA_, TH_, TS_...>::deepEntryGuard(GuardControl& control) noexcept {
	const Prong  requested = compoRequested(control);
	FFSM2_ASSERT(requested < WIDTH);

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepEnter(PlanControl& control) noexcept {
	Prong& requested = compoRequested(control);
	Prong& active	 = compoActive	 (control);

	FFSM2_ASSERT(requested < WIDTH);
	FFSM2_ASSERT(active	  == INVALID_PRONG);

	active	  = requested;
	requested = INVALID_PRONG;

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepReenter(PlanControl& FFSM2_UNUSED(control)) noexcept {
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepPreUpdate(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPreUpdate(control);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS( subStatus(control) |=)
		SubStates::widePreUpdate(control, active);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepUpdate(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepUpdate(control);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS( subStatus(control) |=)
		SubStates::wideUpdate(control, active);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepPostUpdate(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS( subStatus	 (control) |=)
		SubStates::widePostUpdate(control, active);

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPostUpdate(control);
	FFSM2_IF_PLANS(headStatus	 (control) |= h);
}

template <typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepPreReact(FullControl& control,
								   const TEvent& event) noexcept
{
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPreReact(control, event);
	FFSM2_IF_PLANS(headStatus  (control) |= h);

	FFSM2_IF_PLANS( subStatus  (control) |=)
		SubStates::widePreReact(control, event, active);
}

template <typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepReact(FullControl& control,
								const TEvent& event) noexcept
{
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState:: deepReact(control, event);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS( subStatus(control) |=)
		SubStates:: wideReact(control, event, active);
}

template <typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepPostReact(FullControl& control,
									const TEvent& event) noexcept
{
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS( subStatus	(control) |=)
		SubStates::widePostReact(control, event, active);

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPostReact(control, event);
	FFSM2_IF_PLANS(headStatus	(control) |= h);
}

template <typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepQuery(ConstControl& control,
								TEvent& event) const noexcept
{
	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	HeadState::deepQuery(control, event);
	SubStates::wideQuery(control, event, active);
}

#if FFSM2_PLANS_AVAILABLE()

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepUpdatePlans(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	const TaskStatus s =  subStatus(control) |
		SubStates::wideUpdatePlans (control, active);

	const bool planExists = control._core.planData.planExists;

	if (s && planExists)
		control.updatePlan(static_cast<HeadState&>(*this), s);
}

#endif

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
C_<TA_, TH_, TS_...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	FFSM2_ASSERT(compoRequested(control) < WIDTH);

	return SubStates::wideExitGuard(control, active);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
C_<TA_, TH_, TS_...>::deepExitGuard(GuardControl& control) noexcept {
	const Prong  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	FFSM2_ASSERT(compoRequested(control) < WIDTH);

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepExit(PlanControl& control) noexcept {
	Prong& active	 = compoActive   (control);
	FFSM2_ASSERT(active < WIDTH);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	active	  = INVALID_PRONG;

#if FFSM2_PLANS_AVAILABLE()
	Plan plan = control.plan();
	plan.clear();
#endif
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepChangeToRequested(PlanControl& control) noexcept {
	Prong& requested = compoRequested(control);
	Prong& active	 = compoActive	 (control);

	FFSM2_ASSERT(active	   < WIDTH);

	FFSM2_ASSERT(requested < WIDTH);

	if (requested != active) {
		SubStates::wideExit	  (control, active);

		active	  = requested;
		requested = INVALID_PRONG;

		SubStates::wideEnter  (control, active);
	} else {
		requested = INVALID_PRONG;

		// reconstruction done in S_::reenter()
		SubStates::wideReenter(control, active);
	}
}

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepSaveActive(const Registry& registry,
									 WriteStream& stream) const noexcept
{
	stream.template write<WIDTH_BITS>(registry.active);
}

template <typename TA_, typename TH_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
C_<TA_, TH_, TS_...>::deepLoadRequested(Registry& registry,
										ReadStream& stream) const noexcept
{
	Prong& requested = compoRequested(registry);

	requested = stream.template read<WIDTH_BITS>();
	FFSM2_ASSERT(requested < WIDTH);
}

#endif

}
}

namespace ffsm2 {
namespace detail {

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct G_ final {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;
	using Activation		 = TActivation;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long TASK_CAPACITY		  = NTaskCapacity;
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

template <typename>
struct M_;

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct M_<
		   G_<
			   NFeatureTag
			 , TContext
			 , TActivation
			 , NSubstitutionLimit
			 FFSM2_IF_PLANS(, NTaskCapacity)
			 , TPayload
		   >
	   > final
{
	using Cfg = G_<
					NFeatureTag
				  , TContext
				  , TActivation
				  , NSubstitutionLimit
				  FFSM2_IF_PLANS(, NTaskCapacity)
				  , TPayload
				>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root				= RF_<Cfg, CI_<THead, TSubStates...>>;

	/// @brief Headless root
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot			= RF_<Cfg, CI_<void,  TSubStates...>>;

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif
};

}

/// @brief Type configuration for MachineT<>
using Config = detail::G_<
						   FFSM2_FEATURE_TAG
						 , EmptyContext
						 , Automatic
						 , 4
						 FFSM2_IF_PLANS(, INVALID_LONG)
						 , void
					   >;

/// @brief 'Template namespace' for FSM classes
/// @tparam TConfig 'ConfigT<>' type configuration for MachineT<>
/// @see ConfigT<>
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

}

namespace ffsm2 {
namespace detail {

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	typename TConfig
  , typename TApex
>
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

	static_assert(Args::STATE_COUNT	   < static_cast<unsigned>(-1), "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT	  == static_cast<unsigned>(StateList::SIZE), "STATE_COUNT != StateList::SIZE");

	using Core					= CoreT<Args>;

	using Apex					= MaterialT<0, Args, TApex>;

	using ConstControl			= ConstControlT<Args>;
	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Short	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

public:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	/// @brief Transition
	using Transition			= typename Core::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using CPlan					= CPlanT<Args>;
	using Plan					= PayloadPlanT<Args>;

	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	FFSM2_CONSTEXPR(11)	explicit R_(Context& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	FFSM2_CONSTEXPR(11)	explicit R_(PureContext&& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	FFSM2_CONSTEXPR(NO) R_(const R_& )															noexcept = default;
	FFSM2_CONSTEXPR(NO) R_(		 R_&&)															noexcept = default;

	FFSM2_CONSTEXPR(20)	~R_()																	noexcept;

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(14)		  Context& context()												noexcept	{ return _core.context;							}

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(11)	const Context& context()										  const noexcept	{ return _core.context;							}

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID  stateId()														noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Access state instance
	/// @tparam `TState` State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(14)		  TState& access()													noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam `TState` State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(11)	const TState& access()											  const noexcept	{ return static_cast<const TState&>(_apex);		}

	/// @brief Trigger FSM update cycle (recursively call `update()` from the root down to the leaf states
	///   on all active states then process requested transitions)
	FFSM2_CONSTEXPR(14)	void update()															noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states
	///   on all active states then process requested transitions)
	/// @tparam `TEvent` Event type
	/// @param `event` Event to react to
	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void react(const TEvent& event)											noexcept;

	/// @brief Recursively call 'query()' from the root down to the leaf states on all active states
	/// @tparam `TEvent` Event type
	/// @param `event` Event to react to
	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void query(TEvent& event)										  const noexcept;

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_CONSTEXPR(11)	StateID activeStateId()											  const noexcept	{ return _core.registry.active;							}

	/// @brief Check if a state is active
	/// @param `stateId` Destination state identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive(const StateID stateId_)							  const noexcept	{ return _core.registry.active == stateId_;				}

	/// @brief Check if a state is active
	/// @tparam `TState` Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive()													  const noexcept	{ return _core.registry.active == stateId<TState>();	}

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	  Plan plan()															noexcept	{ return  Plan{_core.planData};		}

	/// @brief Access read-only plan
	/// @return Read-only plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.planData};		}

	/// @brief Succeed a plan task for a state
	/// @param `stateId` State identifier
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)									noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void succeed()															noexcept	{ succeed(stateId<TState>());		}

	/// @brief Fail a plan task for a state
	/// @param `stateId` State identifier
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)									noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void fail	()															noexcept	{ fail	 (stateId<TState>());		}

#endif

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @param `stateId` Destination state identifier
	/// @see `immediateChangeTo()`
	FFSM2_CONSTEXPR(14)	void changeTo			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @tparam `TState` Destination state type
	/// @see `immediateChangeTo()`
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo			()												noexcept	{ changeTo (stateId<TState>());		}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @see `changeTo()`
	FFSM2_CONSTEXPR(14)	void immediateChangeTo	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` Destination state type
	/// @see `changeTo()`
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void immediateChangeTo	()												noexcept	{ immediateChangeTo	(stateId<TState>());			}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the transition recorded during last `update()` / `react()`
	/// @return Array of last recorded transitions
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(11)	const Transition& previousTransition()							  const noexcept	{ return _core.previousTransition;	}

	/// @brief Force process a transition (skips `guard()` calls)
	///   Can be used to synchronize multiple FSMs
	/// @param `transition` 'Transition' to replay
	/// @return Success status
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(14)	bool replayTransition(const StateID destination)						noexcept;

#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param `logger` A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)									noexcept	{ _core.logger = logger;			}

#endif

protected:
	FFSM2_CONSTEXPR(14)	void initialEnter()														noexcept;
	FFSM2_CONSTEXPR(14)	void finalExit()														noexcept;

	FFSM2_CONSTEXPR(14)	void processRequest()													noexcept;
	FFSM2_CONSTEXPR(14)	void processTransitions(Transition& currentTransition)					noexcept;

	FFSM2_CONSTEXPR(14)	bool applyRequest(const Transition& currentTransition,
										  const StateID destination)							noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByEntryGuards(const Transition& currentTransition,
													const Transition& pendingTransition)		noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByGuards(const Transition& currentTransition,
											   const Transition& pendingTransition)				noexcept;

#if FFSM2_SERIALIZATION_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void save(WriteStream& stream)									  const noexcept;
	FFSM2_CONSTEXPR(14)	void load( ReadStream& stream)											noexcept;
#endif

protected:
	Core _core;
	Apex _apex;
};

}
}

namespace ffsm2 {
namespace detail {

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(11)
R_<TG_, TA_>::R_(Context& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{context
		  FFSM2_IF_LOG_INTERFACE(, logger)}
{}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(11)
R_<TG_, TA_>::R_(PureContext&& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{move(context)
		  FFSM2_IF_LOG_INTERFACE(, logger)}
{}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(20)
R_<TG_, TA_>::~R_() noexcept {
	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::update() noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	Transition emptyTransition;
	FullControl control{_core, emptyTransition};

	_apex. deepPreUpdate(control);
	_apex.	  deepUpdate(control);
	_apex.deepPostUpdate(control);

#if FFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearRegionStatuses();
#endif

	processRequest();
}

template <typename TG_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::react(const TEvent& event) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	Transition emptyTransition;
	FullControl control{_core, emptyTransition};

	_apex. deepPreReact(control, event);
	_apex.	  deepReact(control, event);
	_apex.deepPostReact(control, event);

#if FFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearRegionStatuses();
#endif

	processRequest();
}

template <typename TG_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::query(TEvent& event) const noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	ConstControl control{_core};

	_apex.deepQuery(control, event);
}

#if FFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::succeed(const StateID stateId_) noexcept {
	_core.planData.tasksSuccesses.set(stateId_);

	FFSM2_LOG_TASK_STATUS(_core.context, stateId_, StatusEvent::SUCCEEDED);
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::fail(const StateID stateId_) noexcept {
	_core.planData.tasksFailures.set(stateId_);

	FFSM2_LOG_TASK_STATUS(_core.context, stateId_, StatusEvent::FAILED);
}

#endif

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::changeTo(const StateID stateId_) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request = Transition{stateId_};

	FFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, stateId_);
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateChangeTo(const StateID stateId_) noexcept {
	changeTo(stateId_);

	processRequest();
}

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::replayTransition(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.previousTransition.clear();

	if (FFSM2_CHECKED(destination != INVALID_SHORT)) {
		Transition currentTransition;
		PlanControl control{_core, currentTransition};

		applyRequest(currentTransition,
					 destination);

		_core.previousTransition = Transition{destination};

		_apex.deepChangeToRequested(control);

		_core.registry.clearRequests();

		FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));

		return true;
	}

	return false;
}

#endif

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::initialEnter() noexcept {
	FFSM2_ASSERT(!_core.registry.isActive());
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	Transition currentTransition;
	Transition pendingTransition;

	PlanControl control{_core, currentTransition};
	applyRequest(currentTransition, 0);

	cancelledByEntryGuards(currentTransition,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _core.request;
		 ++i)
	{
		//backup();

		if (applyRequest(currentTransition,
						 _core.request.destination))
		{
			pendingTransition = _core.request;
			_core.request.clear();

			if (cancelledByEntryGuards(currentTransition,
									   pendingTransition))
				FFSM2_BREAK();
			else
				currentTransition = pendingTransition;

			pendingTransition.clear();
		} else
			_core.request.clear();
	}
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = currentTransition);

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::finalExit() noexcept {
	FFSM2_ASSERT(_core.registry.isActive());
	FFSM2_ASSERT(!_core.request);

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepExit(control);

	_core.registry.clear();
	_core.request .clear();

#if FFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.previousTransition.clear();
#endif
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::processRequest() noexcept {
	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));

	Transition currentTransition;

	if (_core.request) {
		processTransitions(currentTransition);

		FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
	}

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = currentTransition);
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::processTransitions(Transition& currentTransition) noexcept {
	FFSM2_ASSERT(_core.request);

	PlanControl control{_core, currentTransition};

	Transition pendingTransition;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _core.request;
		++i)
	{
		//backup();

		if (applyRequest(currentTransition,
						 _core.request.destination))
		{
			pendingTransition = _core.request;
			_core.request.clear();

			if (cancelledByGuards(currentTransition,
								  pendingTransition))
				;
			else
				currentTransition = pendingTransition;

			pendingTransition.clear();
		} else
			_core.request.clear();
	}
	FFSM2_ASSERT(!_core.request);

	if (currentTransition)
		_apex.deepChangeToRequested(control);

	_core.registry.clearRequests();
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::applyRequest(const Transition& currentTransition,
						   const StateID destination) noexcept
{
	if (currentTransition != Transition{destination}) {
		_core.registry.requested = destination;

		return true;
	} else
		return false;
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::cancelledByEntryGuards(const Transition& currentTransition,
									 const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_core
							, currentTransition
							, pendingTransition};

	return _apex.deepEntryGuard(guardControl);
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::cancelledByGuards(const Transition& currentTransition,
								const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_core
							, currentTransition
							, pendingTransition};

	return _apex.deepForwardExitGuard (guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::save(WriteStream& stream) const noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_apex.deepSaveActive(_core.registry, stream);

	// TODO: save(stream, _core.requests);

#if FFSM2_PLANS_AVAILABLE()
	// TODO: save(stream, _core.planData);
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: save(stream, _core.rng);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	// TODO: save(stream, _core.transitionTarget);
	// TODO: save(stream, _core.previousTransition);
#endif
}

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::load(ReadStream& stream) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.registry.clearRequests();
	_apex.deepLoadRequested(_core.registry, stream);

	_core.request.clear();
	// TODO: load(stream, _core.requests);

#if FFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
	// TODO: load(stream, _core.planData);
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: load(stream, _core.rng);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.previousTransition.clear();
#endif

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepChangeToRequested(control);
}

#endif

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

}
}

namespace ffsm2 {
namespace detail {

// Automatic / manual [de]activation

template <
	typename
  , typename
>
class RV_;

// Automatic enter() / exit()

template <
	FeatureTag NFeatureTag
  , typename TContext
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES RV_<
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			R_ <
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		R_ <
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(14)	RV_(const RV_&  other)													noexcept;
	FFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)													noexcept;

	FFSM2_CONSTEXPR(20)	~RV_()																	noexcept;

#if FFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

private:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;
#endif

private:
	using Base::initialEnter;
	using Base::finalExit;

protected:
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_core;
	using Base::_apex;
#endif
};

// Manual enter() / exit()

template <
	FeatureTag NFeatureTag
  , typename TContext
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES RV_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			R_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		R_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

protected:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::PlanControl;

	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

public:
	using typename Base::Transition;

private:
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
#endif

public:
	using Base::Base;

	/// @brief Check if FSM is active
	/// @return FSM active status
	FFSM2_CONSTEXPR(11)	bool isActive()												  const noexcept	{ return _core.registry.isActive();	}

	using Base::isActive;

	/// @brief Manually start the FSM
	///	  Can be used with UE4 to start / stop the FSM in `BeginPlay()` / `EndPlay()`
	FFSM2_CONSTEXPR(14)	void enter()														noexcept	{ initialEnter();	}

	/// @brief Manually stop the FSM
	///   Can be used with UE4 to start / stop the FSM in `BeginPlay()` / `EndPlay()`
	FFSM2_CONSTEXPR(14)	void exit()															noexcept	{ finalExit();		}

#if FFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///   Can be used with UE4 USTRUCT() NetSerialize() to load replicated FSM from FArchive
	/// @param `destination` Transition destination
	/// @see `FFSM2_ENABLE_TRANSITION_HISTORY`
	FFSM2_CONSTEXPR(14)	void replayEnter(const StateID destination)							noexcept;

#endif

private:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;

	FFSM2_CONSTEXPR(14)	void loadEnter(ReadStream& stream)										noexcept;
#endif

protected:
	using Base::initialEnter;
	using Base::finalExit;

	using Base::_core;

#if FFSM2_SERIALIZATION_AVAILABLE() || FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_apex;
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::applyRequest;
#endif
};

}
}

namespace ffsm2 {
namespace detail {

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(Context& context
																		FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(PureContext&& context
																		FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(const RV_& other) noexcept
	: Base{other}
{}

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(RV_&& other) noexcept
	: Base{move(other)}
{}

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(20)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::~RV_() noexcept {
	finalExit();
}

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	stream.template write<1>(1);
	save(stream);
}

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (FFSM2_CHECKED(stream.template read<1>()))
		Base::load(stream);
}

#endif

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	if (isActive()) {
		stream.template write<1>(1);

		save(stream);
	}
	else
		stream.template write<1>(0);
}

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (stream.template read<1>()) {
		if (isActive())
			Base::load(stream);
		else
			loadEnter (stream);
	}
	else
		if (isActive())
			finalExit();
}

#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::replayEnter(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.active == INVALID_SHORT);
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	Transition currentTransition;
	PlanControl control{_core, currentTransition};

	applyRequest(currentTransition,
				 destination);

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = Transition{destination});

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

#endif

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::loadEnter(ReadStream& stream) noexcept {
	FFSM2_ASSERT(_core.registry.empty());
	_apex.deepLoadRequested(_core.registry, stream);

	FFSM2_ASSERT(!_core.request);

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_ASSERT(_core.planData.empty() == 0);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	FFSM2_ASSERT(_core.transitionTargets  .empty());
	FFSM2_ASSERT(_core.previousTransitions.empty());
#endif

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepEnter(control);
}

#endif

}
}

namespace ffsm2 {
namespace detail {

template <
	typename
  , typename
>
class RP_;

// Non-'void' payloads

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

	using Transition			= TransitionT<TPayload>;

public:
	using typename Base::Payload;

public:
	using Base::Base;
	using Base::processRequest;

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()												noexcept	{ return Base::template stateId<TState>();		}

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	FFSM2_CONSTEXPR(14)	void changeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith				(const Payload& payload)		noexcept	{ changeWith(stateId<TState>(),		 payload );	}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	/// @brief Transition into a state
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `changeWith()`
	FFSM2_CONSTEXPR(14)	void immediateChangeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `changeWith()`
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void immediateChangeWith	(const Payload& payload)		noexcept	{ immediateChangeWith   (stateId<TState>(), payload );	}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

protected:
	using Base::_core;
};

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TApex
>
class FFSM2_EMPTY_BASES RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>
	: public			RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>
{
	using Base =		RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>;

public:
	using Base::Base;
};

}
}

namespace ffsm2 {
namespace detail {

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::changeWith(const StateID  stateId_,
																		   const Payload& payload) noexcept
{
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request = Transition{stateId_, payload};

	FFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, stateId_);
}

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateChangeWith(const StateID  stateId_,
																					const Payload& payload) noexcept
{
	changeWith(stateId_, payload);

	processRequest();
}

}
}

namespace ffsm2 {
namespace detail {

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	typename
  , typename
>
class InstanceT;

// TContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(Context& context
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(11)	explicit InstanceT(PureContext&& context
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(NO) InstanceT(const InstanceT& )												noexcept = default;
	FFSM2_CONSTEXPR(NO) InstanceT(		InstanceT&&)												noexcept = default;

private:
	using Base::_core;
};

// TContext&

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	using Base::Base;

private:
	using Base::_core;
};

// TContext*

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(Context context = nullptr
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(NO)	InstanceT(const InstanceT& )												noexcept = default;
	FFSM2_CONSTEXPR(NO)	InstanceT(		InstanceT&&)												noexcept = default;

	FFSM2_CONSTEXPR(14)	void setContext(Context context)											noexcept	{ _core.context = context; }

private:
	using Base::_core;
};

// TContext == EmptyContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	, public	 EmptyContext
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept;

	using Base::Base;
};

}
}

namespace ffsm2 {
namespace detail {

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(Context& context
																			  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(PureContext&& context
																			  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, TC_*, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(Context context
																			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

template <FeatureTag NFT_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, EmptyContext, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(FFSM2_IF_LOG_INTERFACE(Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

}
}

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

#if _MSC_VER == 1900
	#pragma warning(pop)
#endif

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

//#undef FFSM2_EMPTY_BASES

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

