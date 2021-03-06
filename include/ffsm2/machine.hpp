﻿// FFSM2 (flat state machine for games and interactive applications)
// 0.6.0 (2021-04-11)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2021
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

#define FFSM2_VERSION_MAJOR 0
#define FFSM2_VERSION_MINOR 6
#define FFSM2_VERSION_PATCH 0

#define FFSM2_VERSION (10000 * FFSM2_VERSION_MAJOR + 100 * FFSM2_VERSION_MINOR + FFSM2_VERSION_PATCH)

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef FFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif
#include <utility>			// move(), forward()

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#define FFSM2_INLINE													 //inline

////////////////////////////////////////////////////////////////////////////////

#if INTPTR_MAX == INT64_MAX
	#define FFSM2_64BIT_OR_32BIT(p64, p32)									  p64
#else
	#define FFSM2_64BIT_OR_32BIT(p64, p32)									  p32
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG && defined _MSC_VER
	#define FFSM2_BREAK()										   __debugbreak()
#else
	#define FFSM2_BREAK()											   ((void) 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
	#define FFSM2_IF_DEBUG(...)										  __VA_ARGS__
	#define FFSM2_UNLESS_DEBUG(...)
	#define FFSM2_DEBUG_OR(y, n)												y
#else
	#define FFSM2_IF_DEBUG(...)
	#define FFSM2_UNLESS_DEBUG(...)									  __VA_ARGS__
	#define FFSM2_DEBUG_OR(y, n)												n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_ASSERT
	#define FFSM2_IF_ASSERT(...)									  __VA_ARGS__
	#define FFSM2_CHECKED(x)						(!!(x) || (FFSM2_BREAK(), 0))
	#define FFSM2_ASSERT(x)							(!!(x) || (FFSM2_BREAK(), 0))
	#define FFSM2_ASSERT_OR(y, n)												y
#else
	#define FFSM2_IF_ASSERT(...)
	#define FFSM2_CHECKED(x)													x
	#define FFSM2_ASSERT(x)											   ((void) 0)
	#define FFSM2_ASSERT_OR(y, n)												n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define FFSM2_EXPLICIT_MEMBER_SPECIALIZATION
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_ALL
	#define FFSM2_ENABLE_PLANS
	#define FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_ENABLE_TRANSITION_HISTORY
	#define FFSM2_ENABLE_STRUCTURE_REPORT
#endif

//------------------------------------------------------------------------------

#ifndef FFSM2_DISABLE_TYPEINDEX
	#define FFSM2_IF_TYPEINDEX(...)									  __VA_ARGS__
	#define FFSM2_TYPEINDEX_MASK										 (1 << 0)
#else
	#define FFSM2_IF_TYPEINDEX(...)
	#define FFSM2_TYPEINDEX_MASK										 (0 << 0)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_IF_PLANS(...)										  __VA_ARGS__
	#define FFSM2_PLANS_MASK											 (1 << 1)
#else
	#define FFSM2_IF_PLANS(...)
	#define FFSM2_PLANS_MASK											 (0 << 1)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_IF_SERIALIZATION(...)								  __VA_ARGS__
	#define FFSM2_SERIALIZATION_MASK									 (1 << 2)
#else
	#define FFSM2_IF_SERIALIZATION(...)
	#define FFSM2_SERIALIZATION_MASK									 (0 << 2)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	#define FFSM2_IF_TRANSITION_HISTORY(...)						  __VA_ARGS__
	#define FFSM2_TRANSITION_HISTORY_MASK								 (1 << 3)
#else
	#define FFSM2_IF_TRANSITION_HISTORY(...)
	#define FFSM2_TRANSITION_HISTORY_MASK								 (0 << 3)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_STRUCTURE_REPORT
	#define FFSM2_IF_STRUCTURE_REPORT(...)							  __VA_ARGS__
	#define FFSM2_STRUCTURE_REPORT_MASK									 (1 << 4)
#else
	#define FFSM2_IF_STRUCTURE_REPORT(...)
	#define FFSM2_STRUCTURE_REPORT_MASK									 (0 << 4)
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define FFSM2_ENABLE_LOG_INTERFACE
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								 (1 << 5)
#else
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								 (0 << 5)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	#define FFSM2_IF_LOG_INTERFACE(...)								  __VA_ARGS__
	#define FFSM2_LOG_INTERFACE_MASK									 (1 << 6)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)					\
		if (_logger)															\
			_logger->recordTransition(CONTEXT, ORIGIN, DESTINATION)

#ifdef FFSM2_ENABLE_PLANS

	#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)						\
		if (_logger)															\
			_logger->recordTaskStatus(CONTEXT, ORIGIN, STATUS)

	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)								\
		if (_logger)															\
			_logger->recordPlanStatus(CONTEXT, STATUS)

#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)						\
		if (_logger)															\
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#else

	#define FFSM2_IF_LOG_INTERFACE(...)
	#define FFSM2_LOG_INTERFACE_MASK									 (0 << 6)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)
	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)
#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_VERBOSE_DEBUG_LOG

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)							\
		if (auto* const logger = control._logger)								\
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

#elif defined FFSM2_ENABLE_LOG_INTERFACE

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)							\
		if (auto* const logger = control._logger)								\
			log(METHOD, *logger, control.context(), METHOD_ID)

#else

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

#endif

////////////////////////////////////////////////////////////////////////////////

namespace ffsm2 {

using FeatureTag = uint8_t;

constexpr FeatureTag FFSM2_FEATURE_TAG = FFSM2_TYPEINDEX_MASK
									   | FFSM2_PLANS_MASK
									   | FFSM2_SERIALIZATION_MASK
									   | FFSM2_TRANSITION_HISTORY_MASK
									   | FFSM2_STRUCTURE_REPORT_MASK
									   | FFSM2_VERBOSE_DEBUG_LOG_MASK
									   | FFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef FFSM2_TYPEINDEX_MASK
#undef FFSM2_PLANS_MASK
#undef FFSM2_SERIALIZATION_MASK
#undef FFSM2_TRANSITION_HISTORY_MASK
#undef FFSM2_STRUCTURE_REPORT_MASK
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
#endif

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic" // error : extra ';'
#endif

//------------------------------------------------------------------------------

namespace ffsm2 {

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload {};

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

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <bool B, class TT, class TF>
struct ConditionalT {
	using Type = TT;
};

template <class TT, class TF>
struct ConditionalT<false, TT, TF> {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <bool B, class TT, class TF>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_INLINE void
fill(T& a, const char value) noexcept {
	memset(&a, (int) value, sizeof(a));
}

//------------------------------------------------------------------------------

template <typename TIndex, typename TElement, TIndex NCount>
constexpr TIndex
count(const TElement(&)[NCount]) noexcept {
	return NCount;
}

////////////////////////////////////////////////////////////////////////////////

template <int N1, int N2>
struct Min {
	static constexpr auto VALUE = N1 < N2 ? N1 : N2;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <int N1, int N2>
struct Max {
	static constexpr auto VALUE = N1 > N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <typename T>
constexpr T
min(const T t1, const T t2) noexcept {
	return t1 < t2 ?
		t1 : t2;
}


//------------------------------------------------------------------------------

template <uint64_t NCapacity>
struct UnsignedCapacityT {
	static constexpr uint64_t CAPACITY = NCapacity;

	using Type = Conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 Conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 Conditional<CAPACITY <= UINT32_MAX, uint32_t,
													 uint64_t>>>;
};

template <uint64_t NCapacity>
using UnsignedCapacity = typename UnsignedCapacityT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <uint64_t NBitWidth>
struct UnsignedBitWidthT {
	static constexpr Short BIT_WIDTH = NBitWidth;

	using Type = Conditional<BIT_WIDTH <= 8,  uint8_t,
				 Conditional<BIT_WIDTH <= 16, uint16_t,
				 Conditional<BIT_WIDTH <= 32, uint32_t,
											  uint64_t>>>;

	static_assert(BIT_WIDTH <= 64, "STATIC ASSERT");
};

template <uint64_t NCapacity>
using UnsignedBitWidth = typename UnsignedBitWidthT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <typename T1, typename T2>
constexpr T1
contain(const T1 x, const T2 to) noexcept {
	return (x + (T1) to - 1) / (T1) to;
}

//------------------------------------------------------------------------------

constexpr Short
bitWidth(const Short x) noexcept {
	return x <=   2 ? 1 :
		   x <=   4 ? 2 :
		   x <=   8 ? 3 :
		   x <=  16 ? 4 :
		   x <=  32 ? 5 :
		   x <=  64 ? 6 :
		   x <= 128 ? 7 :
					  8 ;
}

//------------------------------------------------------------------------------

template <typename TTo, typename TFrom>
FFSM2_INLINE void
overwrite(TTo& to, const TFrom& from) noexcept {
	static_assert(sizeof(TTo) == sizeof(TFrom), "STATIC ASSERT");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TO, typename TI>
FFSM2_INLINE TO
convert(const TI& in) noexcept {
	TO out;

	overwrite(out, in);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConst;

template <typename>
struct StaticPrintType;

////////////////////////////////////////////////////////////////////////////////

}
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
	FFSM2_INLINE IteratorT(Container& container,
						   const Index cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const IteratorT<Container>& other) const noexcept;

	FFSM2_INLINE IteratorT& operator ++()		  noexcept;

	FFSM2_INLINE	   Item& operator *()		  noexcept	{ return  _container[_cursor];	}
	FFSM2_INLINE const Item& operator *()	const noexcept	{ return  _container[_cursor];	}

	FFSM2_INLINE	   Item* operator->()		  noexcept	{ return &_container[_cursor];	}
	FFSM2_INLINE const Item* operator->()	const noexcept	{ return &_container[_cursor];	}

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
	FFSM2_INLINE IteratorT(const Container& container,
						   const Index cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const IteratorT<const Container>& other) const noexcept;

	FFSM2_INLINE IteratorT& operator ++()		  noexcept;

	FFSM2_INLINE const Item& operator *()	const noexcept	{ return _container[_cursor];	}

	FFSM2_INLINE const Item* operator->()	const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
IteratorT<T>::operator != (const IteratorT<T>& FFSM2_IF_ASSERT(other)) const noexcept {
	FFSM2_ASSERT(&_container == &other._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
IteratorT<T>&
IteratorT<T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
IteratorT<const T>::operator != (const IteratorT<const T>& FFSM2_IF_ASSERT(other)) const noexcept {
	FFSM2_ASSERT(&_container == &other._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
IteratorT<const T>&
IteratorT<const T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class StaticArrayT {
public:
	using Item  = T;
	using Index = UnsignedCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index DUMMY	= (Index) -1;

public:
	FFSM2_INLINE StaticArrayT() = default;
	FFSM2_INLINE StaticArrayT(const Item filler)					  noexcept;

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i)					  noexcept;

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i)				const noexcept;

	FFSM2_INLINE Index count()									const noexcept	{ return CAPACITY;										}

	FFSM2_INLINE void fill(const Item filler)						  noexcept;
	FFSM2_INLINE void clear()										  noexcept	{ fill(INVALID_SHORT);									}

	FFSM2_INLINE IteratorT<      StaticArrayT>  begin()				  noexcept	{ return IteratorT<      StaticArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const StaticArrayT>  begin()			const noexcept	{ return IteratorT<const StaticArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const StaticArrayT> cbegin()			const noexcept	{ return IteratorT<const StaticArrayT>(*this,     0);	}

	FFSM2_INLINE IteratorT<      StaticArrayT>    end()				  noexcept	{ return IteratorT<      StaticArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const StaticArrayT>    end()			const noexcept	{ return IteratorT<const StaticArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const StaticArrayT>   cend()			const noexcept	{ return IteratorT<const StaticArrayT>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArrayT<T, 0> {
	using Item  = T;

	FFSM2_INLINE StaticArrayT() = default;
	FFSM2_INLINE StaticArrayT(const Item) noexcept {}
};

//------------------------------------------------------------------------------

template <typename T, Long NCapacity>
class ArrayT {
	template <typename>
	friend class IteratorT;

public:
	using Item	= T;
	using Index = UnsignedCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index DUMMY	= (Index) -1;

public:
	FFSM2_INLINE void clear()										  noexcept	{ _count = 0;									}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	FFSM2_INLINE Index append(const TValue&  value)					  noexcept;

	template <typename TValue>
	FFSM2_INLINE Index append(		TValue&& value)					  noexcept;

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i)					  noexcept;

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i)				const noexcept;

	FFSM2_INLINE Index count()									const noexcept	{ return _count;	}

	FFSM2_INLINE ArrayT& operator += (const Item& item)				  noexcept;
	FFSM2_INLINE ArrayT& operator += (	   Item&& item)				  noexcept;

	template <Long N>
	FFSM2_INLINE ArrayT& operator += (const ArrayT<Item, N>& other)	  noexcept;

	FFSM2_INLINE IteratorT<      ArrayT>  begin()					  noexcept	{ return IteratorT<		 ArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const ArrayT>  begin()				const noexcept	{ return IteratorT<const ArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const ArrayT> cbegin()				const noexcept	{ return IteratorT<const ArrayT>(*this,     0);	}

	FFSM2_INLINE IteratorT<      ArrayT>	end()					  noexcept	{ return IteratorT<		 ArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const ArrayT>	end()				const noexcept	{ return IteratorT<const ArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const ArrayT>   cend()				const noexcept	{ return IteratorT<const ArrayT>(*this, DUMMY);	}

private:
	FFSM2_INLINE Index next(const Index i)						const noexcept	{ return i + 1;		}
	FFSM2_INLINE Index limit()									const noexcept	{ return _count;	}

private:
	Index _count = 0;

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	Item _items[CAPACITY]; // warning 4324 triggers for 'StructureStateInfo'

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class ArrayT<T, 0> {
public:
	using Item	= T;
	using Index = UnsignedCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
StaticArrayT<T, NC>::StaticArrayT(const Item filler) noexcept {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
StaticArrayT<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
StaticArrayT<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Index i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::append(const TValue& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::append(TValue&& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
ArrayT<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
ArrayT<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------
// SPECIFIC

template <typename T, Long NC>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const Item& item) noexcept {
	append(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (Item&& item) noexcept {
	append(std::move(item));

	return *this;
}

// SPECIFIC
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	inline Units(Short unit_  = INVALID_SHORT,
				 Short width_ = INVALID_SHORT) noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

//------------------------------------------------------------------------------

template <typename TIndex, Short NCapacity>
class BitArrayT final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_WIDTH = sizeof(Unit) * 8;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, UNIT_WIDTH);

public:
	BitArrayT() noexcept {
		clear();
	}

	FFSM2_INLINE void clear()								  noexcept;

	FFSM2_INLINE bool get  (const Index index)			const noexcept;
	FFSM2_INLINE void set  (const Index index)				  noexcept;
	FFSM2_INLINE void clear(const Index index)				  noexcept;

private:
	Unit _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArrayT<TIndex, 0> final {
public:
	FFSM2_INLINE void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::clear() noexcept {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
bool
BitArrayT<TI, NC>::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::set(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::clear(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
#ifdef FFSM2_ENABLE_SERIALIZATION

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

	FFSM2_INLINE void clear()										  noexcept;

	FFSM2_INLINE	   Data& data()									  noexcept	{ return _data;		}
	FFSM2_INLINE const Data& data()								const noexcept	{ return _data;		}

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
	FFSM2_INLINE explicit BitWriteStreamT(Buffer& buffer,
										  const Long cursor = 0)	  noexcept;

	template <Short NBitWidth>
	FFSM2_INLINE void write(const UnsignedBitWidth<NBitWidth> item)	  noexcept;

	FFSM2_INLINE Long cursor()									const noexcept	{ return _cursor;	}

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
	FFSM2_INLINE explicit BitReadStreamT(const Buffer& buffer,
										 const Long cursor = 0)		  noexcept;

	template <Short NBitWidth>
	FFSM2_INLINE UnsignedBitWidth<NBitWidth> read()					  noexcept;

	FFSM2_INLINE Long cursor()									const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef FFSM2_ENABLE_SERIALIZATION

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
void
StreamBufferT<NBC>::clear() noexcept {
	fill(_data, 0);
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
BitWriteStreamT<NBC>::BitWriteStreamT(Buffer& buffer,
									  const Long cursor) noexcept
	: _buffer{buffer}
	, _cursor{cursor}
{
	_buffer.clear();
}

//------------------------------------------------------------------------------

template <Long NBC>
template <Short NBitWidth>
void
BitWriteStreamT<NBC>::write(const UnsignedBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
BitReadStreamT<NBC>::BitReadStreamT(const Buffer& buffer,
									const Long cursor) noexcept
	: _buffer{buffer}
	, _cursor{cursor}
{}

//------------------------------------------------------------------------------

template <Long NBC>
template <Short NBitWidth>
UnsignedBitWidth<NBitWidth>
BitReadStreamT<NBC>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor <= BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item item = 0;
	Short itemCursor = 0;

	for (Short itemWidth = BIT_WIDTH; itemWidth; )
		if (FFSM2_CHECKED(_cursor + itemWidth <= BIT_CAPACITY)) {
			const Long	byteIndex		= _cursor >> 3;
			const uint8_t& byte			= _buffer._data[byteIndex];

			const Short byteChunkStart	= _cursor & 0x7;
			const Short byteDataWidth	= 8 - byteChunkStart;
			const Short byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
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
struct Type {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TL_
	: Type<Ts>...
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
struct PrependT<T, TL_<Ts...>> {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using PrependTypes = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <typename, typename>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TL_<Ts1...>, TL_<Ts2...>> {
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
struct LowerT<NHalf, NIndex, TFirst, TRest...> {
	using LTypeList = typename LowerT<NHalf, NIndex + 1, TRest...>::Type;

	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependTypes<TFirst, LTypeList>,
					 LTypeList
				 >;
};

template <Long NHalf, Long NIndex>
struct LowerT<NHalf, NIndex> {
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
struct UpperT<NHalf, NIndex, TFirst, TRest...> {
	using Type = Conditional<
					 (NIndex < NHalf),
					 UpperTypes<NHalf, NIndex + 1, TRest...>,
					 TL_<TFirst, TRest...>
				 >;
};

template <Long NHalf, Long NIndex>
struct UpperT<NHalf, NIndex> {
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
struct Find<TL_<Ts...>, T>
	: FindImpl<0, T, Ts...>
{};

////////////////////////////////////////////////////////////////////////////////
// SPECIFIC

}

template <typename TList, typename T>
constexpr Long index   () noexcept { return detail::Find<TList, T>::VALUE;					}

template <typename TList, typename T>
constexpr bool contains() noexcept { return std::is_base_of<detail::Type<T>, TList>::value;	}

// SPECIFIC
//------------------------------------------------------------------------------

}

namespace ffsm2 {

////////////////////////////////////////////////////////////////////////////////

enum class Method : uint8_t {
	NONE,

	ENTRY_GUARD,
	CONSTRUCT,
	ENTER,
	REENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,
	DESTRUCT,

#ifdef FFSM2_ENABLE_PLANS
	PLAN_SUCCEEDED,
	PLAN_FAILED,
#endif

	COUNT
};

#ifdef FFSM2_ENABLE_PLANS

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

#ifndef FFSM2_DISABLE_TYPEINDEX

static inline
const char*
stateName(const std::type_index stateType) noexcept {
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

static inline
const char*
methodName(const Method method) noexcept {
	switch (method) {
	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::CONSTRUCT:		 return "construct";
	case Method::REENTER:		 return "reenter";
	case Method::UPDATE:		 return "update";
	case Method::REACT:			 return "react";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";
	case Method::DESTRUCT:		 return "destruct";

#ifdef FFSM2_ENABLE_PLANS
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
	constexpr TransitionBase() noexcept = default;

	//----------------------------------------------------------------------

	inline TransitionBase(const StateID destination_) noexcept
		: destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline TransitionBase(const StateID origin_,
						  const StateID destination_) noexcept
		: origin	 {origin_}
		, destination{destination_}
	{}

	//----------------------------------------------------------------------

	inline bool operator == (const TransitionBase& other) const noexcept {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	//----------------------------------------------------------------------

	inline explicit operator bool() const noexcept {
		return destination != INVALID_STATE_ID;
	}

	//----------------------------------------------------------------------

	inline void clear() noexcept {
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
struct alignas(4) TransitionT
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 4>::type;

	using TransitionBase::TransitionBase;

	//----------------------------------------------------------------------

	FFSM2_INLINE TransitionT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionT(const StateID destination,
							 const Payload& payload) noexcept
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionT(const StateID destination,
							 Payload&& payload) noexcept
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	FFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 const Payload& payload) noexcept
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 Payload&& payload) noexcept
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	FFSM2_INLINE bool operator == (const TransitionT& other) const noexcept {
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload ==  other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	bool payloadSet = false;
	Storage storage;
};

//------------------------------------------------------------------------------

template <>
struct alignas(4) TransitionT<void>
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}

struct Request {
	Short index;
};

////////////////////////////////////////////////////////////////////////////////

}
namespace ffsm2 {

#ifdef FFSM2_ENABLE_LOG_INTERFACE

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
		, typename TContext = EmptyContext>
struct LoggerInterfaceT {
	using Context		 = TContext;

	using Method		 = ::ffsm2::Method;
	using StateID		 = ::ffsm2::StateID;

#ifdef FFSM2_ENABLE_PLANS
	using StatusEvent	 = ::ffsm2::StatusEvent;
#endif

	virtual void recordMethod(Context& /*context*/,
							  const StateID /*origin*/,
							  const Method /*method*/) noexcept
	{}

	virtual void recordTransition(Context& /*context*/,
								  const StateID /*origin*/,
								  const StateID /*target*/) noexcept
	{}

#ifdef FFSM2_ENABLE_PLANS

	virtual void recordTaskStatus(Context& /*context*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/) noexcept
	{}

	virtual void recordPlanStatus(Context& /*context*/,
								  const StatusEvent /*event*/) noexcept
	{}

#endif

	virtual void recordCancelledPending(Context& /*context*/,
										const StateID /*origin*/) noexcept
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

#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskBase {
	FFSM2_INLINE TaskBase() noexcept {}

	FFSM2_INLINE TaskBase(const StateID origin_,
						  const StateID destination_) noexcept
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

inline bool operator == (const TaskBase& lhs, const TaskBase& rhs) noexcept {
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination;
}

//------------------------------------------------------------------------------

template <typename TPayload>
struct TaskT
	: TaskBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 2>::type;

	using TaskBase::TaskBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TaskT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TaskT(const StateID origin,
					   const StateID destination,
					   const Payload& payload) noexcept
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TaskT(const StateID origin,
					   const StateID destination,
					   Payload&& payload) noexcept
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	Storage storage;
	bool payloadSet = false;
};

//------------------------------------------------------------------------------

template <>
struct TaskT<void>
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload, Long NCapacity>
class TaskListT {
public:
	using Index = Long;

	static constexpr Index CAPACITY = NCapacity;

	static constexpr Index INVALID = Index (-1);

private:
	using Payload	= TPayload;
	using Item		= TaskT<Payload>;

public:
	template <typename... TArgs>
	Index emplace(TArgs&&... args)												  noexcept;

	void remove(const Index i)													  noexcept;

	FFSM2_INLINE	   Item& operator[] (const Index i)							  noexcept;
	FFSM2_INLINE const Item& operator[] (const Index i)						const noexcept;

	FFSM2_INLINE Index count()												const noexcept	{ return _count;	}

private:
	FFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)	const noexcept);

private:
	Item _items[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last  = 0;
	Index _count = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class TaskListT<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TP, Long NC>
template <typename... TA>
Long
TaskListT<TP, NC>::emplace(TA&&... args) noexcept {
	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		auto& cell = _items[index];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			FFSM2_ASSERT(cell.prev == INVALID);
			FFSM2_ASSERT(cell.next != INVALID);

			_vacantHead = cell.next;

			auto& head = _items[_vacantHead];
			FFSM2_ASSERT(head.prev == index);
			head.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& vacant = _items[_vacantHead];
			vacant.prev = INVALID;
			vacant.next = INVALID;
		} else {
			FFSM2_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		FFSM2_IF_ASSERT(verifyStructure());

		new (&cell) Item{std::forward<TA>(args)...};

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
void
TaskListT<TP, NC>::remove(const Index i) noexcept {
	FFSM2_ASSERT(i < CAPACITY && _count);

	auto& fresh = _items[i];

	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		fresh.prev = INVALID;
		fresh.next = _vacantHead;

		auto& head = _items[_vacantHead];
		head.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		FFSM2_ASSERT(_count		 == CAPACITY);
		FFSM2_ASSERT(_vacantHead == INVALID);
		FFSM2_ASSERT(_vacantTail == INVALID);

		fresh.prev = INVALID;
		fresh.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	FFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TP, Long NC>
const typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) const noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TP, Long NC>
void
TaskListT<TP, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		FFSM2_ASSERT(_items[_vacantHead].prev == INVALID);
		FFSM2_ASSERT(_items[_vacantTail].next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			FFSM2_ASSERT(occupied != c);

			const auto& current = _items[c];

			const auto f = current.next;
			if (f != INVALID) {
				// next
				const auto& following = _items[f];

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
#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskLink {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds {
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
					 , TPayload>>
{
	using StateList		= TStateList;
	using Payload		= TPayload;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<Payload>;
	using Tasks			= TaskListT<Payload, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads		= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBits		= BitArrayT<StateID, StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	void clearTaskStatus(const StateID stateId) noexcept;
	void verifyEmptyStatus(const StateID stateId) const noexcept;

#ifdef FFSM2_ENABLE_ASSERT
	void verifyPlans() const noexcept;
	Long verifyPlan()  const noexcept;
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
					 , void>>
{
	using StateList		= TStateList;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= TaskListT<void, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBits		= BitArrayT<StateID, StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	void clearTaskStatus(const StateID stateId) noexcept;
	void verifyEmptyStatus(const StateID stateId) const noexcept;

#ifdef FFSM2_ENABLE_ASSERT
	void verifyPlans() const noexcept;
	Long verifyPlan()  const noexcept;
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef FFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
Long
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlan() const noexcept {
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
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef FFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
Long
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlan() const noexcept {
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

struct Status {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	inline Status(const Result result_ = Result::NONE) noexcept;

	inline explicit operator bool()					const noexcept	{ return result != Result::NONE;			}

	inline void clear() noexcept;
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) noexcept {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct IteratorT {
		FFSM2_INLINE IteratorT(const CPlanT& plan)		  noexcept;

		FFSM2_INLINE explicit operator bool()		const noexcept;

		FFSM2_INLINE void operator ++()					  noexcept;

		FFSM2_INLINE const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next()					const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_INLINE CPlanT(const PlanData& planData)		  noexcept;

	template <typename T>
	static constexpr StateID  stateId()					  noexcept	{ return			index<StateList , T>();	}

public:
	FFSM2_INLINE explicit operator bool()			const noexcept;

	FFSM2_INLINE IteratorT first()						  noexcept	{ return IteratorT{*this};					}

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

	struct IteratorT {
		FFSM2_INLINE IteratorT(PlanBaseT& plan)			  noexcept;

		FFSM2_INLINE explicit operator bool()		const noexcept;

		FFSM2_INLINE void operator ++()					  noexcept;

		FFSM2_INLINE	   Task& operator  *()			  noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE	   Task* operator ->()			  noexcept	{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE void remove()						  noexcept;

		FFSM2_INLINE Long next()					const noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator {
		FFSM2_INLINE CIterator(const PlanBaseT& plan)	  noexcept;

		FFSM2_INLINE explicit operator bool()		const noexcept;

		FFSM2_INLINE void operator ++()					  noexcept;

		FFSM2_INLINE	   Task& operator  *()			  noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE	   Task* operator ->()			  noexcept	{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next()					const noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_INLINE PlanBaseT(PlanData& planData)			  noexcept;

	template <typename T>
	static constexpr StateID  stateId()					  noexcept	{ return			index<StateList , T>();	}

	bool append(const StateID origin,
				const StateID destination)				  noexcept;

	bool linkTask(const Long index)						  noexcept;

public:
	FFSM2_INLINE explicit operator bool()			const noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_INLINE void clear()							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool change(const StateID origin, const StateID destination)	  noexcept	{ return append(origin, destination);							}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool change(const StateID destination)							  noexcept	{ return change(stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool change()													  noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return IteratorT to the first task
	FFSM2_INLINE  IteratorT first()						  noexcept	{ return  IteratorT{*this};										}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_INLINE CIterator first()					const noexcept { return CIterator{*this};										}

private:
	void remove(const Long task)						  noexcept;

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

	bool append(const StateID origin,
				const StateID destination,
				const Payload& payload) noexcept;

	bool append(const StateID origin,
				const StateID destination,
				Payload&& payload) noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith(const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 ,  		 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith(const StateID origin, const StateID destination,	   Payload&& payload) noexcept	{ return append(origin								 , destination								 , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith(					   const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , 			 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith(					   const StateID destination,	   Payload&& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith(												  const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), 			 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith(													   Payload&& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), std::move(payload));	}

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

Status::Status(const Result result_) noexcept
	: result{result_}
{}

//------------------------------------------------------------------------------

void
Status::clear() noexcept {
	result = Result::NONE;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
CPlanT<TArgs>::IteratorT::IteratorT(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::IteratorT::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
CPlanT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
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
CPlanT<TArgs>::CPlanT(const PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::operator bool() const noexcept {
	if (_bounds.first < TASK_CAPACITY) {
		FFSM2_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		FFSM2_ASSERT(_bounds.last == INVALID_LONG);
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::IteratorT::IteratorT(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::IteratorT::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::IteratorT::remove() noexcept {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
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
PlanBaseT<TArgs>::CIterator::CIterator(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::CIterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
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
PlanBaseT<TArgs>::PlanBaseT(PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
PlanBaseT<TArgs>::operator bool() const noexcept {
	if (_bounds.first < TASK_CAPACITY) {
		FFSM2_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		FFSM2_ASSERT(_bounds.last == INVALID_LONG);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
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
bool
PlanBaseT<TArgs>::linkTask(const Long index) noexcept {
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
void
PlanBaseT<TArgs>::clear() noexcept {
	if (_bounds.first < TaskLinks::CAPACITY) {
		FFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			FFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const auto& taskLink = _planData.taskLinks[index];
			FFSM2_ASSERT(index == _bounds.first ?
							 taskLink.prev == INVALID_LONG :
							 taskLink.prev <  TaskLinks::CAPACITY);

			const Long next = taskLink.next;

			_planData.tasks.remove(index);

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
void
PlanBaseT<TArgs>::remove(const Long task) noexcept {
	FFSM2_ASSERT(_planData.planExists);
	FFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
	FFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

	FFSM2_ASSERT(task < TaskLinks::CAPACITY);

	TaskLink& curr = _planData.taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		FFSM2_ASSERT(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		FFSM2_ASSERT(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	curr.prev = INVALID_LONG;
	curr.next = INVALID_LONG;

	_planData.tasks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
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
bool
PlanT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																			   const StateID destination,
																			   Payload&& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, std::move(payload)));
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

struct Registry {
	FFSM2_INLINE bool isActive() const noexcept {
		return active != INVALID_SHORT;
	}

	FFSM2_INLINE void clearRequests() noexcept {
		requested = INVALID_SHORT;
	}

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

#ifdef FFSM2_ENABLE_PLANS
	using PlanData			= PlanDataT<TArgs>;
	using CPlan				= CPlanT<TArgs>;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		FFSM2_INLINE Origin(ControlT& control_,
							const StateID stateId)					  noexcept;

		FFSM2_INLINE ~Origin()										  noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE ControlT(Context& context
						, Registry& registry
						, Transition& request
						FFSM2_IF_PLANS(, PlanData& planData)
						FFSM2_IF_TRANSITION_HISTORY(, const Transition& previousTransition)
						FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
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
	static constexpr StateID stateId()								  noexcept	{ return index<StateList, TState>();			}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_INLINE	   Context& _()									  noexcept	{ return _context;								}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_INLINE const Context& _()								const noexcept	{ return _context;								}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_INLINE	   Context& context()							  noexcept	{ return _context;								}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_INLINE const Context& context()						const noexcept	{ return _context;								}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	FFSM2_INLINE const Transition& request()					const noexcept	{ return _request;								}

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Access read-only plan
	/// @return Plan
	FFSM2_INLINE CPlan plan()									const noexcept	{ return CPlan{_planData};						}

#endif

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransition()'
	/// @return Array of last transition requests
	FFSM2_INLINE const Transition& previousTransition()			const noexcept	{ return _previousTransition;					}

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

#ifdef FFSM2_ENABLE_PLANS
	using typename Control::PlanData;
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;
#endif

	using Control::Control;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_PLANS
// COMMON

	/// @brief Access plan
	/// @return Plan
	FFSM2_INLINE  Plan plan()										  noexcept	{ return  Plan{_planData};	}

// COMMON
// COMMON

	/// @brief Access read-only plan
	/// @return Read-only plan
	FFSM2_INLINE CPlan plan()									const noexcept	{ return CPlan{_planData};	}

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

#ifdef FFSM2_ENABLE_PLANS
	using TasksBits		= BitArrayT<StateID, StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		FFSM2_INLINE Lock(FullControlBaseT& control_)				  noexcept;
		FFSM2_INLINE ~Lock()										  noexcept;

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
	FFSM2_INLINE void changeTo(const StateID stateId)				  noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_INLINE void changeTo()									  noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Succeed a plan task for the current state
	FFSM2_INLINE void succeed()										  noexcept;

	/// @brief Fail a plan task for the current state
	FFSM2_INLINE void fail()										  noexcept;

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

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef FFSM2_ENABLE_PLANS

	template <typename TState>
	void updatePlan(TState& headState, const Status subStatus)		  noexcept;

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
	FFSM2_INLINE void changeWith(const StateID  stateId,
								 const Payload& payload)	  noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
									  Payload&& payload)	  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(const Payload& payload)	  noexcept	{ changeWith(FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(	  Payload&& payload)	  noexcept	{ changeWith(FullControlBase::template stateId<TState>(), std::move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_UTILITY_THEORY
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

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef FFSM2_ENABLE_PLANS

	template <typename TState>
	void updatePlan(TState& headState, const Status subStatus)		  noexcept;

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

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControl::PlanData;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE GuardControlT(Context& context
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

	FFSM2_INLINE Transition currentTransition()			const noexcept	{ return _currentTransition;	}

	/// @brief Get pending transition requests
	/// @return ArrayT of pending transition requests
	FFSM2_INLINE Transition& pendingTransition()		const noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	FFSM2_INLINE void cancelPendingTransition()				  noexcept;

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
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId) noexcept
	: control{control_}
	, prevId{control._originId}
{
	control._originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

// COMMON
////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// COMMON
//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
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
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId,
																										   Payload&& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// COMMON
////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
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

	using Control		= ControlT<TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#ifdef FFSM2_ENABLE_PLANS
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	FFSM2_INLINE void preEntryGuard(Context&)		  noexcept {}

	FFSM2_INLINE void preEnter	   (Context&)		  noexcept {}
	FFSM2_INLINE void preReenter   (Context&)		  noexcept {}

	FFSM2_INLINE void preUpdate	   (Context&)		  noexcept {}

	template <typename TEvent>
	FFSM2_INLINE void preReact	   (const TEvent&,
									Context&)		  noexcept {}

	FFSM2_INLINE void preExitGuard (Context&)		  noexcept {}

	FFSM2_INLINE void postExit	   (Context&)		  noexcept {}

	template <typename T>
	static constexpr StateID  stateId()				  noexcept { return index<StateList, T>();	}
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct B_<TFirst, TRest...>
	: TFirst
	, B_<TRest...>
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#ifdef FFSM2_ENABLE_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void widePreEntryGuard(Context& context)		 noexcept;

	FFSM2_INLINE void widePreEnter	   (Context& context)		 noexcept;
	FFSM2_INLINE void widePreReenter   (Context& context)		 noexcept;

	FFSM2_INLINE void widePreUpdate	   (Context& context)		 noexcept;

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context)		 noexcept;

	FFSM2_INLINE void widePreExitGuard (Context& context)		 noexcept;

	FFSM2_INLINE void widePostExit	   (Context& context)		 noexcept;
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

#ifdef FFSM2_ENABLE_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void entryGuard	   (GuardControl&)			  noexcept {}

	FFSM2_INLINE void enter			   (PlanControl&)			  noexcept {}
	FFSM2_INLINE void reenter		   (PlanControl&)			  noexcept {}

	FFSM2_INLINE void update		   (FullControl&)			  noexcept {}

	template <typename TEvent>
	FFSM2_INLINE void react			   (const TEvent&,
										FullControl&)			  noexcept {}

	FFSM2_INLINE void exitGuard		   (GuardControl&)			  noexcept {}

	FFSM2_INLINE void exit			   (PlanControl&)			  noexcept {}

#ifdef FFSM2_ENABLE_PLANS
	FFSM2_INLINE void planSucceeded	   (FullControl&)			  noexcept {}
	FFSM2_INLINE void planFailed	   (FullControl&)			  noexcept {}
#endif

	FFSM2_INLINE void widePreEntryGuard(Context& context)		  noexcept;

	FFSM2_INLINE void widePreEnter	   (Context& context)		  noexcept;
	FFSM2_INLINE void widePreReenter   (Context& context)		  noexcept;

	FFSM2_INLINE void widePreUpdate	   (Context& context)		  noexcept;

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context)		  noexcept;

	FFSM2_INLINE void widePreExitGuard (Context& context)		  noexcept;

	FFSM2_INLINE void widePostExit	   (Context& context)		  noexcept;
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Dynamic_ {};

template <typename... TI>
struct DB_
	: Dynamic_
	, B_<TI...>
{};

template <typename TArgs>
using DynamicEmptyT = DB_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Static_ {};

template <typename... TI>
struct SB_
	: Static_
	, B_<TI...>
{};

template <typename TArgs>
using StaticEmptyT = SB_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
void
B_<TF, TR...>::widePreReact(const TEvent& event,
							Context& context) noexcept
{
	TF::preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePostExit(Context& context) noexcept {
	TF::postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
void
B_<TF>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
void
B_<TF>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
void
B_<TF>::widePreReact(const TEvent& event,
					 Context& context) noexcept
{
	TF::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePostExit(Context& context) noexcept {
	TF::postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
	template <typename TArgs>
	static FFSM2_INLINE void execute(ffsm2::detail::GuardControlT<TArgs>&) noexcept {}
};

namespace detail {

//------------------------------------------------------------------------------

FFSM2_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DynamicBox final {
	using Type = T;

	static constexpr bool isBare()							  noexcept	{ return false;								}

	union {
		Type t_;
	};

	FFSM2_INLINE  DynamicBox()								  noexcept	{}
	FFSM2_INLINE ~DynamicBox()								  noexcept	{}

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control)	  noexcept	{ Guard<Type>::execute(control);			}

	FFSM2_INLINE void construct()							  noexcept;
	FFSM2_INLINE void  destruct()							  noexcept;

	FFSM2_INLINE	   Type& get()							  noexcept	{ FFSM2_ASSERT(initialized_); return t_;	}
	FFSM2_INLINE const Type& get()						const noexcept	{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_TYPEINDEX(FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type)));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare() noexcept	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control)	  noexcept;

	FFSM2_INLINE void construct()							  noexcept	{}
	FFSM2_INLINE void  destruct()							  noexcept	{}

	FFSM2_INLINE	   Type& get()							  noexcept	{ return t_;								}
	FFSM2_INLINE const Type& get()						const noexcept	{ return t_;								}

	FFSM2_IF_TYPEINDEX(FFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Type)));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = Conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DynamicBox<T, TArgs>,
					 StaticBox <T, TArgs>
				 >;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
DynamicBox<T, TA>::construct() noexcept {
	FFSM2_ASSERT(!initialized_);

	new(&t_) T{};

	FFSM2_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
void
DynamicBox<T, TA>::destruct() noexcept {
	FFSM2_ASSERT(initialized_);

	t_.~T();

	FFSM2_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
StaticBox<T, TA>::guard(GuardControlT<TA>& control) noexcept {
	t_.widePreEntryGuard(control.context());
	t_.		  entryGuard(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  typename THead>
struct S_ final {
	static constexpr auto STATE_ID	 = NStateId;

	using Context		= typename TArgs::Context;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TArgs::Logger;
#endif

	using Control		= ControlT<TArgs>;

	using PlanControl	= PlanControlT<TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;
	using HeadBox		= Boxify<Head, TArgs>;

	//----------------------------------------------------------------------

	FFSM2_INLINE bool	deepEntryGuard		 (GuardControl& control) noexcept;

	FFSM2_INLINE void	deepConstruct		 (PlanControl&  control) noexcept;

	FFSM2_INLINE void	deepEnter			 (PlanControl&  control) noexcept;
	FFSM2_INLINE void	deepReenter			 (PlanControl&  control) noexcept;

	FFSM2_INLINE Status deepUpdate			 (FullControl&  control) noexcept;

	template <typename TEvent>
	FFSM2_INLINE Status deepReact			 (FullControl&	control,
											  const TEvent& event)	 noexcept;

	FFSM2_INLINE bool	deepExitGuard		 (GuardControl& control) noexcept;

	FFSM2_INLINE void	deepExit			 (PlanControl&	control) noexcept;

	FFSM2_INLINE void	deepDestruct		 (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_PLANS
	FFSM2_INLINE void	wrapPlanSucceeded	 (FullControl&	control) noexcept;
	FFSM2_INLINE void	wrapPlanFailed		 (FullControl&	control) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	deepChangeToRequested(Control&)				 noexcept {}

	//----------------------------------------------------------------------

#if defined _DEBUG || defined FFSM2_ENABLE_STRUCTURE_REPORT || defined FFSM2_ENABLE_LOG_INTERFACE

	static constexpr Long NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Empty			= EmptyT<TArgs>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename THost, typename... TParams>
	void log(TReturn(THost::*)(TParams...),
			 Logger& logger,
			 Context& context,
			 const Method method) const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	void log(TReturn(Empty::*)(TParams...),
			 Logger&,
			 Context&,
			 const Method) const noexcept
	{}

#endif

	//----------------------------------------------------------------------

	// TODO: account for boxing
	//
	// if you see..
	// VS	 - error C2079: 'ffsm2::detail::S_<...>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'ffsm2::detail::S_<...>::Head' (aka 'Blah')
	//
	// .. add definition for the state 'Blah'
	HeadBox _headBox;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID N, typename TA, typename TH>
bool
S_<N, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.guard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepConstruct(PlanControl&
						 #if defined FFSM2_ENABLE_PLANS || defined FFSM2_ENABLE_LOG_INTERFACE
							 control
						 #endif
							 ) noexcept {
	FFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepEnter(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepReenter(PlanControl& control) noexcept {
	FFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.destruct ();
	_headBox.construct();

	_headBox.get().widePreReenter(control.context());
	_headBox.get().		  reenter(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
Status
S_<N, TA, TH>::deepUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreUpdate(control.context());
	_headBox.get().		  update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
template <typename TEvent>
Status
S_<N, TA, TH>::deepReact(FullControl& control,
						 const TEvent& event) noexcept
{
	auto reaction = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	FFSM2_LOG_STATE_METHOD(reaction,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreReact(event, control.context());
	(_headBox.get().*reaction) (event, control);				//_headBox.get().react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
bool
S_<N, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.get().widePreExitGuard(control.context());
	_headBox.get().		  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepExit(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from ffsm2::Machine::Instance::State
	_headBox.get().		   exit(control);
	_headBox.get().widePostExit(control.context());
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepDestruct(PlanControl&
						#if defined FFSM2_ENABLE_LOG_INTERFACE || defined FFSM2_ENABLE_PLANS
							control
						#endif
							) noexcept
{
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::DESTRUCT);

	_headBox.destruct();

#ifdef FFSM2_ENABLE_PLANS
	control._planData.clearTaskStatus(STATE_ID);
#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planFailed(control);
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

template <typename...>
struct CSI_;

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct CSI_<TInitial>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> {
	using Type = SI_<TH>;
};

template <typename TH, typename... TS>
struct WrapInfoT< CI_<TH, TS...>> {
	using Type =  CI_<TH, TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------
// COMMON

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;

	static constexpr Short WIDTH			  = 1;

	static constexpr Long  STATE_COUNT	 	  = StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH			  = sizeof...(TSubStates);

#ifdef FFSM2_ENABLE_SERIALIZATION
	static constexpr Long  WIDTH_BITS	  = bitWidth(WIDTH);
	static constexpr Long  ACTIVE_BITS	  = WIDTH_BITS;
#endif

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;

#ifdef FFSM2_ENABLE_SERIALIZATION
	static constexpr Short SERIAL_BITS		  = NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_PLANS
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload		= TPayload;

#ifdef FFSM2_ENABLE_SERIALIZATION
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

template <StateID, typename, Short, typename...>
struct CS_;

template <typename, typename>
class RC_;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct MaterialT;

template <StateID N, typename TA, typename TH>
struct MaterialT   <N, TA, TH> {
	using Type = S_<N, TA, TH>;
};

template <StateID N, typename TA, 			 typename... TS>
struct MaterialT   <N, TA, CI_<void,         TS...>> {
	using Type = C_<   TA, StaticEmptyT<TA>, TS...>;
};

template <StateID N, typename TA, typename TH, typename... TS>
struct MaterialT   <N, TA, CI_<TH,			  TS...>> {
	using Type = C_<   TA, TH,				  TS...>;
};

template <StateID N, typename... TS>
using Material = typename MaterialT<N, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT = TConfig::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_PLANS
	static constexpr Long  TASK_CAPACITY	  = TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload		= typename TConfig::Payload;
	using Transition	= TransitionT<Payload>;

#ifdef FFSM2_ENABLE_PLANS
	using Task			= typename TConfig::Task;
#endif

#ifdef FFSM2_ENABLE_SERIALIZATION
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

	using DynamicState	= DynamicEmptyT<Args>;

	template <typename... TInjections>
	using DynamicStateT	= DB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using StaticState	= StaticEmptyT<Args>;

	template <typename... TInjections>
	using StaticStateT	= SB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using State			= StaticState;

	template <typename... TInjections>
	using StateT		= StaticStateT<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_LOG_INTERFACE
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
struct CSubMaterialT;

template <StateID N, typename TA, Short NI, typename... TS>
struct CSubMaterialT<N, TA, NI, TL_<TS...>> {
	using Type = CS_<N, TA, NI,		TS...>;
};

template <StateID N, typename TA, Short NI, typename TL>
using CSubMaterial = typename CSubMaterialT<N, TA, NI, TL>::Type;

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
struct CS_ final {
	static_assert(sizeof...(TStates) >= 2, "");

	static constexpr StateID  INITIAL_ID  = NStateId;

	static constexpr Short	  PRONG_INDEX = NIndex;

	using Args			= TArgs;

#ifdef FFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr Short	  L_PRONG	  = PRONG_INDEX;

	using LStateList	= LHalfTypes<TStates...>;
	using LMaterial		= CSubMaterial<INITIAL_ID,
									   Args,
									   L_PRONG,
									   LStateList>;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using RStateList	= RHalfTypes<TStates...>;
	using RMaterial		= CSubMaterial<INITIAL_ID + LStateList::SIZE,
									   Args,
									   R_PRONG,
									   RStateList>;

	//----------------------------------------------------------------------

	FFSM2_INLINE bool	wideEntryGuard		 (GuardControl& control, const Short prong) noexcept;

	FFSM2_INLINE void	wideConstruct		 (PlanControl&  control, const Short prong) noexcept;

	FFSM2_INLINE void	wideEnter			 (PlanControl&  control, const Short prong) noexcept;
	FFSM2_INLINE void	wideReenter			 (PlanControl&  control, const Short prong) noexcept;

	FFSM2_INLINE Status wideUpdate			 (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	FFSM2_INLINE Status wideReact			 (FullControl&  control,
											  const TEvent& event,	 const Short prong) noexcept;

	FFSM2_INLINE bool	wideExitGuard		 (GuardControl& control, const Short prong) noexcept;

	FFSM2_INLINE void	wideExit			 (PlanControl&  control, const Short prong) noexcept;

	FFSM2_INLINE void	wideDestruct		 (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	wideChangeToRequested(PlanControl&  control, const Short prong) noexcept;

	//----------------------------------------------------------------------

	LMaterial lHalf;
	RMaterial rHalf;
};

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename TState>
struct CS_<NStateId, TArgs, NIndex, TState> final {
	static constexpr StateID INITIAL_ID	= NStateId;

	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<INITIAL_ID, Args, TState>;

	//----------------------------------------------------------------------

	FFSM2_INLINE bool	wideEntryGuard		 (GuardControl& control, const Short prong) noexcept;

	FFSM2_INLINE void	wideConstruct		 (PlanControl&  control, const Short prong) noexcept;

	FFSM2_INLINE void	wideEnter			 (PlanControl&  control, const Short prong) noexcept;
	FFSM2_INLINE void	wideReenter			 (PlanControl&  control, const Short prong) noexcept;

	FFSM2_INLINE Status wideUpdate			 (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	FFSM2_INLINE Status wideReact			 (FullControl&  control,
											  const TEvent& event,	 const Short prong) noexcept;

	FFSM2_INLINE bool	wideExitGuard		 (GuardControl& control, const Short prong) noexcept;

	FFSM2_INLINE void	wideExit			 (PlanControl&  control, const Short prong) noexcept;

	FFSM2_INLINE void	wideDestruct		 (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	wideChangeToRequested(PlanControl&  control, const Short prong) noexcept;

	//----------------------------------------------------------------------

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID N, typename TA, Short NI, typename... TS>
bool
CS_<N, TA, NI, TS...>::wideEntryGuard(GuardControl& control,
									  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
void
CS_<N, TA, NI, TS...>::wideConstruct(PlanControl& control,
									 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideConstruct(control, prong);
	else
		rHalf.wideConstruct(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
void
CS_<N, TA, NI, TS...>::wideEnter(PlanControl& control,
								 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, Short NI, typename... TS>
void
CS_<N, TA, NI, TS...>::wideReenter(PlanControl& control,
								   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
Status
CS_<N, TA, NI, TS...>::wideUpdate(FullControl& control,
								  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
template <typename TEvent>
Status
CS_<N, TA, NI, TS...>::wideReact(FullControl& control,
								 const TEvent& event,
								 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID N, typename TA, Short NI, typename... TS>
bool
CS_<N, TA, NI, TS...>::wideExitGuard(GuardControl& control,
									 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
void
CS_<N, TA, NI, TS...>::wideExit(PlanControl& control,
								const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
void
CS_<N, TA, NI, TS...>::wideDestruct(PlanControl& control,
									const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideDestruct(control, prong);
	else
		rHalf.wideDestruct(control, prong);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID N, typename TA, Short NI, typename... TS>
void
CS_<N, TA, NI, TS...>::wideChangeToRequested(PlanControl& control,
											 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideChangeToRequested(control, prong);
	else
		rHalf.wideChangeToRequested(control, prong);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID N, typename TA, Short NI, typename T>
bool
CS_<N, TA, NI, T>::wideEntryGuard(GuardControl& control,
								  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideConstruct(PlanControl& control,
								 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideEnter(PlanControl& control,
							 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideReenter(PlanControl& control,
							   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
Status
CS_<N, TA, NI, T>::wideUpdate(FullControl& control,
							  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
template <typename TEvent>
Status
CS_<N, TA, NI, T>::wideReact(FullControl& control,
							 const TEvent& event,
							 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID N, typename TA, Short NI, typename T>
bool
CS_<N, TA, NI, T>::wideExitGuard(GuardControl& control,
								 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideExit(PlanControl& control,
							const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideDestruct(PlanControl& control,
								const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepDestruct(control);
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
struct C_ final {
	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Head			= THead;
	using HeadState		= S_<INVALID_STATE_ID, Args, Head>;

	using SubStates		= CS_<0, Args, 0, TSubStates...>;

#ifdef FFSM2_ENABLE_SERIALIZATION
	using Info			= CI_<Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	//----------------------------------------------------------------------

	FFSM2_INLINE bool deepForwardEntryGuard(GuardControl& control) noexcept;
	FFSM2_INLINE bool deepEntryGuard	   (GuardControl& control) noexcept;

	FFSM2_INLINE void deepConstruct		   (PlanControl&  control) noexcept;

	FFSM2_INLINE void deepEnter			   (PlanControl&  control) noexcept;

	FFSM2_INLINE void deepUpdate		   (FullControl&  control) noexcept;

	template <typename TEvent>
	FFSM2_INLINE void deepReact			   (FullControl&  control,
											const TEvent& event)   noexcept;

	FFSM2_INLINE bool deepForwardExitGuard (GuardControl& control) noexcept;
	FFSM2_INLINE bool deepExitGuard		   (GuardControl& control) noexcept;

	FFSM2_INLINE void deepExit			   (PlanControl&  control) noexcept;

	FFSM2_INLINE void deepDestruct		   (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void deepChangeToRequested(PlanControl&  control) noexcept;

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	FFSM2_INLINE void deepSaveActive	   (const Registry& registry, WriteStream& stream) const noexcept;
	FFSM2_INLINE void deepLoadRequested	   (	  Registry& registry, ReadStream&  stream) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.active != INVALID_SHORT);

	const Short  requested  = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _subStates.wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepConstruct(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   == INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	active	  = requested;
	requested = INVALID_SHORT;

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (_headState.deepUpdate(control)) {
		ControlLock lock{control};

		_subStates.wideUpdate(control, active);
	} else {
		FFSM2_IF_PLANS(const Status subStatus =)
		_subStates.wideUpdate(control, active);

	#ifdef FFSM2_ENABLE_PLANS
		if (subStatus && control._planData.planExists)
			control.updatePlan(_headState, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
void
C_<TA, TH, TS...>::deepReact(FullControl& control,
							 const TEvent& event) noexcept
{
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (_headState.deepReact(control, event)) {
		ControlLock lock{control};

		_subStates.wideReact(control, event, active);
	} else {
		FFSM2_IF_PLANS(const Status subStatus =)
		_subStates.wideReact(control, event, active);

	#ifdef FFSM2_ENABLE_PLANS
		if (subStatus && control._planData.planExists)
			control.updatePlan(_headState, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	const Short  active  = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	return _subStates.wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepDestruct(PlanControl& control) noexcept {
	Short& active = control._registry.active;

	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideDestruct(control, active);
	_headState.deepDestruct(control);

	active = INVALID_SHORT;

#ifdef FFSM2_ENABLE_PLANS
	auto plan = control.plan();
	plan.clear();
#endif
}

//------------------------------------------------------------------------------
// COMMON

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   != INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	if (requested != active) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		active	  = requested;
		requested = INVALID_SHORT;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else {
		requested = INVALID_SHORT;

		// reconstruction done in S_::reenter()
		_subStates.wideReenter	(control, active);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepSaveActive(const Registry& registry,
								  WriteStream& stream) const noexcept
{
	stream.template write<WIDTH_BITS>(registry.active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepLoadRequested(Registry& registry,
									 ReadStream& stream) const noexcept
{
	registry.requested = stream.template read<WIDTH_BITS>();
	FFSM2_ASSERT(registry.requested < WIDTH);
}

#endif

// COMMON
////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_PLANS
	static constexpr Long TASK_CAPACITY		 = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#ifdef FFSM2_ENABLE_PLANS
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , Activation, SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Select manual activation strategy
	using ManualActivation	 = G_<FEATURE_TAG, Context, Manual	  , SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#ifdef FFSM2_ENABLE_UTILITY_THEORY
#endif

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, Activation, N				   FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#ifdef FFSM2_ENABLE_PLANS

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
struct M_	   <G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>> {
	using Cfg = G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

#ifdef FFSM2_ENABLE_UTILITY_THEORY
#endif

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root			 = RF_<Cfg, CI_<THead, TSubStates...>>;

	/// @brief Headless root
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot		 = RF_<Cfg, CI_<void,  TSubStates...>>;

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_UTILITY_THEORY
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
	using Apex					= TApex;

	using Forward				= RF_<TConfig, Apex>;
	using StateList				= typename Forward::StateList;
	using Args					= typename Forward::Args;

	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	using MaterialApex			= Material<0, Args, Apex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_PLANS
	using PlanData				= PlanDataT<Args>;
#endif

#ifdef FFSM2_ENABLE_SERIALIZATION
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	/// @brief Transition
	using Transition			= typename Control::Transition;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	FFSM2_INLINE explicit R_(Context& context
						   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	FFSM2_INLINE ~R_() noexcept;

	//----------------------------------------------------------------------

	/// @brief Access context
	/// @return context
	FFSM2_INLINE	   Context& context()								  noexcept	{ return _context;								}

	/// @brief Access context
	/// @return context
	FFSM2_INLINE const Context& context()							const noexcept	{ return _context;								}

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									  noexcept	{ return index<StateList, TState>();			}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update() noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_INLINE void react(const TEvent& event)						  noexcept;

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_INLINE StateID activeStateId()							const noexcept	{ return _registry.active;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	FFSM2_INLINE bool isActive(const StateID stateId)				const noexcept	{ return _registry.active == stateId;			}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_INLINE bool isActive()									const noexcept	{ return _registry.active == stateId<TState>();	}

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	FFSM2_INLINE void changeTo(const StateID stateId)					  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_INLINE void changeTo()										  noexcept	{ changeTo (stateId<TState>());					}

	// COMMON
	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION

	/// @brief Buffer for serialization
	/// @see https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	/// @see FFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer 'SerialBuffer' to serialize to
	/// @see FFSM2_ENABLE_SERIALIZATION
	void save(		SerialBuffer& buffer)							const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer 'SerialBuffer' to de-serialize from
	/// @see FFSM2_ENABLE_SERIALIZATION
	void load(const SerialBuffer& buffer)								  noexcept;

#endif

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Get the transition recorded during last 'update()' / 'react()'
	/// @return Array of last recorded transitions
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	const Transition previousTransition()							const noexcept	{ return _previousTransition;					}

	/// @brief Force process a transition (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param destination Transition destination
	/// @return Success status
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	bool replayTransition(const StateID destination)					  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_INLINE void attachLogger(Logger* const logger)				  noexcept	{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

protected:
	void initialEnter()													  noexcept;
	void finalExit()													  noexcept;

	void processTransitions(Transition& currentTransition) noexcept;

	void applyRequest(const StateID destination)						  noexcept;

	bool cancelledByEntryGuards(const Transition& currentTransition,
								const Transition& pendingTransition)	  noexcept;

	bool cancelledByGuards(const Transition& currentTransition,
						   const Transition& pendingTransition)			  noexcept;

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	Transition _previousTransition;
#endif

	Context _context;

	Registry _registry;
	FFSM2_IF_PLANS(PlanData _planData);

	Transition _request;

	MaterialApex _apex;

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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename Base::Logger;
#endif

public:
	FFSM2_INLINE explicit RV_(Context& context
							FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	FFSM2_INLINE ~RV_() noexcept;

private:
	using Base::initialEnter;
	using Base::finalExit;
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

protected:
	using typename Base::Payload;

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	using typename Base::PlanControl;
	using typename Base::Transition;
#endif

public:
	using Base::Base;

	/// @brief Manually start the FSM
	///   Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	FFSM2_INLINE void enter()								  noexcept;

	/// @brief Manually stop the FSM
	///   Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	FFSM2_INLINE void exit()								  noexcept;

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Start the FSM from a specific state
	///   Can be used with UE4 USTRUCT() NetSerialize() to load replicated FSM from FArchive
	/// @param destination Transition destination
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_INLINE void replayEnter(const StateID destination)  noexcept;

#endif

private:
	using Base::initialEnter;
	using Base::finalExit;

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	using Base::applyRequest;

	using Base::_previousTransition;

	using Base::_context;
	using Base::_registry;
	#ifdef FFSM2_ENABLE_PLANS
		using Base::_planData;
	#endif
	using Base::_request;
	using Base::_apex;
	#ifdef FFSM2_ENABLE_LOG_INTERFACE
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
	static constexpr StateID stateId()					 	  noexcept	{ return Base::template stateId<TState>();				}

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
								 const Payload& payload)	  noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
									  Payload&& payload)	  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(const Payload& payload)	  noexcept	{ changeWith(stateId<TState>(),			  payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(	  Payload&& payload)	  noexcept	{ changeWith(stateId<TState>(), std::move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_UTILITY_THEORY
#endif

protected:
	using Base::_context;
	using Base::_registry;

private:
	using Base::_request;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename Base::Logger;
#endif

public:
	FFSM2_INLINE explicit RC_(Context& context
							FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	FFSM2_INLINE void setContext(const Context&  context)	  noexcept { _context =			  context ; }
	FFSM2_INLINE void setContext(	   Context&& context)	  noexcept { _context = std::move(context); }

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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename Base::Logger;
#endif

public:
	FFSM2_INLINE explicit RC_(Context context
							FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	FFSM2_INLINE void setContext(Context context)			  noexcept { _context = context; }

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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename Base::Logger;
#endif

public:
	FFSM2_INLINE explicit RC_(Context context = nullptr
							FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	FFSM2_INLINE void setContext(Context context)			  noexcept { _context = context; }

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
class RC_			<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, EmptyContext
{
	using Base = RP_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename Base::Logger;
#endif

public:
	explicit FFSM2_INLINE RC_(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<EmptyContext&>(*this)
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
R_<TG, TA>::R_(Context& context
			 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() noexcept {
	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
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
void
R_<TG, TA>::changeTo(const StateID stateId) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION

template <typename TG, typename TA>
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

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

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

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);
}

#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY

template <typename TG, typename TA>
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

		applyRequest(destination);
		_previousTransition = Transition{destination};

		_apex.deepChangeToRequested(control);

		_registry.clearRequests();

		FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
		FFSM2_IF_STRUCTURE_REPORT(udpateActivity());

		return true;
	}

	return false;
}

#endif

//------------------------------------------------------------------------------

template <typename TG, typename TA>
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

	applyRequest(0);

	Transition currentTransition;
	Transition pendingTransition;

	cancelledByEntryGuards(currentTransition,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _request;
		 ++i)
	{
		//backup();

		applyRequest(_request.destination);
		pendingTransition = _request;
		_request.clear();

		if (cancelledByEntryGuards(currentTransition,
								   pendingTransition))
			FFSM2_BREAK();
		else
			currentTransition = pendingTransition;

		pendingTransition.clear();
	}
	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
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

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
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

		applyRequest(_request.destination);
		pendingTransition = _request;
		_request.clear();

		if (cancelledByGuards(currentTransition, pendingTransition))
			;
		else
			currentTransition = pendingTransition;

		pendingTransition.clear();
	}

	if (currentTransition)
		_apex.deepChangeToRequested(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::applyRequest(const StateID destination) noexcept {
	_registry.requested = destination;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG, typename TA>
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

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(Context& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::~RV_() noexcept {
	finalExit();
}

//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::enter() noexcept {
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::exit() noexcept {
	finalExit();
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
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

	applyRequest(destination);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = Transition{destination});

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
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
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																		 Payload&& payload) noexcept
{
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId, std::move(payload)};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

// COMMON
//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_UTILITY_THEORY
#endif

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

#undef FFSM2_INLINE

#undef FFSM2_64BIT_OR_32BIT

//#undef FFSM2_BREAK

#undef FFSM2_IF_DEBUG
#undef FFSM2_UNLESS_DEBUG
#undef FFSM2_DEBUG_OR

#undef FFSM2_IF_ASSERT
#undef FFSM2_CHECKED
#undef FFSM2_ASSERT
#undef FFSM2_ASSERT_OR

#undef FFSM2_EXPLICIT_MEMBER_SPECIALIZATION

#undef FFSM2_IF_TYPEINDEX
#undef FFSM2_IF_PLANS
#undef FFSM2_IF_SERIALIZATION
#undef FFSM2_IF_TRANSITION_HISTORY
#undef FFSM2_IF_STRUCTURE_REPORT
#undef FFSM2_IF_LOG_INTERFACE

#undef FFSM2_LOG_TRANSITION
#undef FFSM2_LOG_TASK_STATUS
#undef FFSM2_LOG_PLAN_STATUS
#undef FFSM2_LOG_CANCELLED_PENDING

#undef FFSM2_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////
