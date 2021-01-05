// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2020
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

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#include <typeindex>
#include <utility>			// std::conditional<>, move(), forward()

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

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_ENABLE_DYNAMIC_PLANS
	#define FFSM2_ENABLE_STATIC_PLANS
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	#define FFSM2_ENABLE_ANY_PLANS

	#define FFSM2_IF_DYNAMIC_PLANS(...)								  __VA_ARGS__
	#define FFSM2_DYNAMIC_PLANS_MASK									 (1 << 0)
#else
	#define FFSM2_IF_DYNAMIC_PLANS(...)
	#define FFSM2_DYNAMIC_PLANS_MASK									 (0 << 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER && _MSC_VER < 1910
	#undef FFSM2_ENABLE_STATIC_PLANS
#endif

#ifdef FFSM2_ENABLE_STATIC_PLANS
	#define FFSM2_ENABLE_ANY_PLANS

	#define FFSM2_IF_STATIC_PLANS(...)								  __VA_ARGS__
	#define FFSM2_STATIC_PLANS_MASK										 (1 << 1)
#else
	#define FFSM2_IF_STATIC_PLANS(...)
	#define FFSM2_STATIC_PLANS_MASK										 (0 << 1)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_ANY_PLANS
	#define FFSM2_IF_ANY_PLANS(...)									  __VA_ARGS__
#else
	#define FFSM2_IF_ANY_PLANS(...)
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

	#ifdef FFSM2_ENABLE_ANY_PLANS

		#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)					\
			if (_logger)														\
				_logger->recordTaskStatus(CONTEXT, ORIGIN, STATUS)

		#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)							\
			if (_logger)														\
				_logger->recordPlanStatus(CONTEXT, STATUS)

	#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)						\
		if (_logger)															\
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#else

	#define FFSM2_IF_LOG_INTERFACE(...)
	#define FFSM2_LOG_INTERFACE_MASK									 (0 << 6)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)

	#ifdef FFSM2_ENABLE_ANY_PLANS
		#define FFSM2_LOG_TASK_STATUS(CONTEXT, ORIGIN, STATUS)
		#define FFSM2_LOG_PLAN_STATUS(CONTEXT, STATUS)
	#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_VERBOSE_DEBUG_LOG

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)							\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

#elif defined FFSM2_ENABLE_LOG_INTERFACE

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)							\
		if (auto* const logger = control.logger())								\
			log(METHOD, *logger, control.context(), METHOD_ID)

#else

	#define FFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

#endif

////////////////////////////////////////////////////////////////////////////////

namespace ffsm2 {

using FeatureTag = uint8_t;

constexpr FeatureTag FFSM2_FEATURE_TAG = FFSM2_DYNAMIC_PLANS_MASK
									   | FFSM2_STATIC_PLANS_MASK
									   | FFSM2_SERIALIZATION_MASK
									   | FFSM2_TRANSITION_HISTORY_MASK
									   | FFSM2_STRUCTURE_REPORT_MASK
									   | FFSM2_VERBOSE_DEBUG_LOG_MASK
									   | FFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef FFSM2_DYNAMIC_PLANS_MASK
#undef FFSM2_STATIC_PLANS_MASK
#undef FFSM2_SERIALIZATION_MASK
#undef FFSM2_TRANSITION_HISTORY_MASK
#undef FFSM2_STRUCTURE_REPORT_MASK
#undef FFSM2_VERBOSE_DEBUG_LOG_MASK
#undef FFSM2_LOG_INTERFACE_MASK

////////////////////////////////////////////////////////////////////////////////

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

template <typename T, unsigned NCount>
constexpr unsigned
count(const T(&)[NCount]) noexcept {
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

template <unsigned NCapacity>
struct UnsignedCapacityT {
	static constexpr Long CAPACITY = NCapacity;

	using Type = Conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 Conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 Conditional<CAPACITY <= UINT32_MAX, uint32_t,
													 uint64_t>>>;

	static_assert(CAPACITY <= UINT64_MAX, "STATIC ASSERT");
};

template <unsigned NCapacity>
using UnsignedCapacity = typename UnsignedCapacityT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <unsigned NBitWidth>
struct UnsignedBitWidthT {
	static constexpr Short BIT_WIDTH = NBitWidth;

	using Type = Conditional<BIT_WIDTH <= 8,  uint8_t,
				 Conditional<BIT_WIDTH <= 16, uint16_t,
				 Conditional<BIT_WIDTH <= 32, uint32_t,
											  uint64_t>>>;

	static_assert(BIT_WIDTH <= 64, "STATIC ASSERT");
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
using UnsignedBitWidth = typename UnsignedBitWidthT<NCapacity>::Type;

//------------------------------------------------------------------------------

constexpr Long
roundUp(const Long x,
		const Long to) noexcept
{
	return (x + (to - 1)) / to;
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

//------------------------------------------------------------------------------

template <unsigned V1, unsigned V2>
struct StaticAssertEquality;

template <unsigned V1>
struct StaticAssertEquality<V1, V1> {};

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;

	template <typename T, Long NCapacity>
	friend class Array;

private:
	FFSM2_INLINE Iterator(Container& container,
						  const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const Iterator<Container>& dummy) const noexcept;

	FFSM2_INLINE Iterator& operator ++()		noexcept;

	FFSM2_INLINE	   Item& operator *()		noexcept	{ return  _container[_cursor];	}
	FFSM2_INLINE const Item& operator *() const noexcept	{ return  _container[_cursor];	}

	FFSM2_INLINE	   Item* operator->()		noexcept	{ return &_container[_cursor];	}
	FFSM2_INLINE const Item* operator->() const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Long _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, Long NCapacity>
	friend class Array;

private:
	FFSM2_INLINE Iterator(const Container& container,
						  const Long cursor)	noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const Iterator<const Container>& dummy) const noexcept;

	FFSM2_INLINE Iterator& operator ++()		noexcept;

	FFSM2_INLINE const Item& operator *() const noexcept	{ return _container[_cursor];	}

	FFSM2_INLINE const Item* operator->() const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
Iterator<T>::operator != (const Iterator<T>& FFSM2_IF_ASSERT(dummy)) const noexcept {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
Iterator<T>&
Iterator<T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
Iterator<const T>::operator != (const Iterator<const T>& FFSM2_IF_ASSERT(dummy)) const noexcept {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
Iterator<const T>&
Iterator<const T>::operator ++() noexcept {
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
class StaticArray {
public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item  = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	FFSM2_INLINE StaticArray() = default;
	FFSM2_INLINE StaticArray(const Item filler)				noexcept;

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i)			noexcept;

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i)	  const noexcept;

	FFSM2_INLINE Long count() const							noexcept	{ return CAPACITY;									}

	FFSM2_INLINE void fill(const Item filler)				noexcept;
	FFSM2_INLINE void clear()								noexcept	{ fill(INVALID_SHORT);								}

	FFSM2_INLINE Iterator<      StaticArray>  begin()		noexcept	{ return Iterator<      StaticArray>(*this,     0); }
	FFSM2_INLINE Iterator<const StaticArray>  begin() const	noexcept	{ return Iterator<const StaticArray>(*this,     0); }
	FFSM2_INLINE Iterator<const StaticArray> cbegin() const	noexcept	{ return Iterator<const StaticArray>(*this,     0); }

	FFSM2_INLINE Iterator<      StaticArray>    end()		noexcept	{ return Iterator<      StaticArray>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const StaticArray>    end() const	noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const StaticArray>   cend() const	noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	FFSM2_INLINE StaticArray() = default;
	FFSM2_INLINE StaticArray(const Item) noexcept {}
};

//------------------------------------------------------------------------------

template <typename T, Long NCapacity>
class Array {
	template <typename>
	friend class Iterator;

public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	FFSM2_INLINE void clear()									  noexcept	{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	FFSM2_INLINE Long append(const TValue& value)				  noexcept;

	template <typename TValue>
	FFSM2_INLINE Long append(	  TValue&& value)				  noexcept;

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i)				  noexcept;

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i)			const noexcept;

	FFSM2_INLINE Long count()								const noexcept	{ return _count;	}

	FFSM2_INLINE Array& operator += (const Item& item)			  noexcept;
	FFSM2_INLINE Array& operator += (	  Item&& item)			  noexcept;

	template <Long N>
	FFSM2_INLINE Array& operator += (const Array<Item, N>& other) noexcept;

	FFSM2_INLINE Iterator<      Array>  begin()					  noexcept	{ return Iterator<		Array>(*this,     0);	}
	FFSM2_INLINE Iterator<const Array>  begin()				const noexcept	{ return Iterator<const Array>(*this,     0);	}
	FFSM2_INLINE Iterator<const Array> cbegin()				const noexcept	{ return Iterator<const Array>(*this,     0);	}

	FFSM2_INLINE Iterator<      Array>	  end()					  noexcept	{ return Iterator<		Array>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const Array>	  end()				const noexcept	{ return Iterator<const Array>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const Array>   cend()				const noexcept	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	FFSM2_INLINE Long next(const Long i)					const noexcept	{ return i + 1;		}
	FFSM2_INLINE Long limit()								const noexcept	{ return _count;	}

private:
	Long _count = 0;

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
class Array<T, 0> {
public:
	static constexpr Long CAPACITY = 0;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<0>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
StaticArray<T, NC>::StaticArray(const Item filler) noexcept {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
StaticArray<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
StaticArray<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
void
StaticArray<T, NC>::fill(const Item filler) noexcept {
	for (Long i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
Long
Array<T, NC>::append(const TValue& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
Long
Array<T, NC>::append(TValue&& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
Array<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
Array<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
Array<T, NC>&
Array<T, NC>::operator += (const Item& item) noexcept {
	append(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
Array<T, NC>&
Array<T, NC>::operator += (Item&& item) noexcept {
	append(std::move(item));

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
Array<T, NC>&
Array<T, NC>::operator += (const Array<T, N>& other) noexcept {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

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
class BitArray final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index CAPACITY	= NCapacity;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, Short>
		friend class BitArray;

	private:
		FFSM2_INLINE explicit Bits(Unit* const storage,
								   const Index width)	  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		FFSM2_INLINE explicit operator bool()		const noexcept;

		FFSM2_INLINE void clear()						  noexcept;

		FFSM2_INLINE bool get  (const Index index)	const noexcept;
		FFSM2_INLINE void set  (const Index index)		  noexcept;
		FFSM2_INLINE void clear(const Index index)		  noexcept;

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <typename, Short>
		friend class BitArray;

	private:
		FFSM2_INLINE explicit CBits(const Unit* const storage,
									const Index width)	  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		FFSM2_INLINE explicit operator bool()		const noexcept;

		FFSM2_INLINE bool get(const Index index)	const noexcept;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() noexcept {
		clear();
	}

	FFSM2_INLINE void clear()							  noexcept;

	FFSM2_INLINE bool get  (const Index index)		const noexcept;
	FFSM2_INLINE void set  (const Index index)			  noexcept;
	FFSM2_INLINE void clear(const Index index)			  noexcept;

	template <Short NUnit, Short NWidth>
	FFSM2_INLINE  Bits bits()							  noexcept;

	template <Short NUnit, Short NWidth>
	FFSM2_INLINE CBits bits()						const noexcept;

	FFSM2_INLINE  Bits bits(const Units& units)			  noexcept;
	FFSM2_INLINE CBits bits(const Units& units)		const noexcept;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	FFSM2_INLINE void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
BitArray<TI, NC>::Bits::operator bool() const noexcept {
	const Short fullUnits = _width / (sizeof(Unit) * 8);

	// TODO: cover this case
	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % (sizeof(Unit) * 8);
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
void
BitArray<TI, NC>::Bits::clear() noexcept {
	const Index count = (_width + 7) / (sizeof(Unit) * 8);

	for (Index i = 0; i < count; ++i)
		_storage[i] = Unit{0};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArray<TI, NC>::Bits::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::Bits::set(const Index index) noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::Bits::clear(const Index index) noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
BitArray<TI, NC>::CBits::operator bool() const noexcept {
	const Short fullUnits = _width / (sizeof(Unit) * 8);

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % (sizeof(Unit) * 8);
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArray<TI, NC>::CBits::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
void
BitArray<TI, NC>::clear() noexcept {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
bool
BitArray<TI, NC>::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::set(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::clear(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
typename BitArray<TI, NC>::Bits
BitArray<TI, NC>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
typename BitArray<TI, NC>::CBits
BitArray<TI, NC>::bits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
typename BitArray<TI, NC>::Bits
BitArray<TI, NC>::bits(const Units& units) noexcept {
	FFSM2_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
typename BitArray<TI, NC>::CBits
BitArray<TI, NC>::bits(const Units& units) const noexcept {
	FFSM2_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return CBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, Long NCapacity>
class List {
public:
	using Index = Long;

	static constexpr Index CAPACITY = NCapacity;

	static constexpr Index INVALID = Index (-1);

private:
	using Item  = TItem;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Links {
		Index prev;
		Index next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	union Cell {
		Item item;
		Links links;

		FFSM2_INLINE Cell() noexcept
			: links{}
		{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	template <typename... TArgs>
	Index emplace(TArgs... args) noexcept;

	void remove(const Index i) noexcept;

	FFSM2_INLINE	   Item& operator[] (const Index i)		  noexcept;
	FFSM2_INLINE const Item& operator[] (const Index i) const noexcept;

	FFSM2_INLINE Index count() const noexcept						{ return _count;	}

private:
	FFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID) const noexcept);

private:
	Cell _cells[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last  = 0;
	Index _count = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class List<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename... TA>
Long
List<T, NC>::emplace(TA... args) noexcept {
	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		auto& cell = _cells[index];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			FFSM2_ASSERT(cell.links.prev == INVALID);
			FFSM2_ASSERT(cell.links.next != INVALID);

			_vacantHead = cell.links.next;

			auto& head = _cells[_vacantHead];
			FFSM2_ASSERT(head.links.prev == index);
			head.links.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& vacant = _cells[_vacantHead];
			vacant.links.prev = INVALID;
			vacant.links.next = INVALID;
		} else {
			FFSM2_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		FFSM2_IF_ASSERT(verifyStructure());

		new (&cell.item) Item{std::forward<TA>(args)...};

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

template <typename T, Long NC>
void
List<T, NC>::remove(const Index i) noexcept {
	FFSM2_ASSERT(i < CAPACITY && _count);

	auto& fresh = _cells[i];

	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		fresh.links.prev = INVALID;
		fresh.links.next = _vacantHead;

		auto& head = _cells[_vacantHead];
		head.links.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		FFSM2_ASSERT(_count		 == CAPACITY);
		FFSM2_ASSERT(_vacantHead == INVALID);
		FFSM2_ASSERT(_vacantTail == INVALID);

		fresh.links.prev = INVALID;
		fresh.links.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	FFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
T&
List<T, NC>::operator[] (const Index i) noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
const T&
List<T, NC>::operator[] (const Index i) const noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename T, Long NC>
void
List<T, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		FFSM2_ASSERT(_vacantHead < CAPACITY);
		FFSM2_ASSERT(_vacantTail < CAPACITY);

		FFSM2_ASSERT(_cells[_vacantHead].links.prev == INVALID);
		FFSM2_ASSERT(_cells[_vacantTail].links.next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			FFSM2_ASSERT(occupied != c);

			const auto& current = _cells[c];

			const auto f = current.links.next;
			if (f != INVALID) {
				// next
				const auto& following = _cells[f];

				FFSM2_ASSERT(following.links.prev == c);

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
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if 0

template<Long N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<Long... Ns>
struct IndexSequence {
	using Type = IndexSequence<Ns...>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename, typename>
struct MakeIndexSequenceT {};

template<Long N, Long... Ns>
struct MakeIndexSequenceT<IndexConstant<N>,
						  IndexSequence<Ns...>>
	: MakeIndexSequenceT<IndexConstant<N - 1>,
						 IndexSequence<N - 1, Ns...>>
{};

template<Long... Ns>
struct MakeIndexSequenceT<IndexConstant<0>,
							  IndexSequence<Ns...>>
	: IndexSequence<Ns...>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<Long N>
using MakeIndexSequence = typename MakeIndexSequenceT<IndexConstant<N>,
													  IndexSequence<>>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct ITL_EntryT {};

template <typename T, Long N>
struct ITL_EntryN
	: ITL_EntryT<T>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct ITL_Impl;

template <Long... Ns, typename... Ts>
struct ITL_Impl<IndexSequence<Ns...>, Ts...>
	: ITL_EntryN<Ts, Ns>...
{
	template <typename T, Long N>
	static constexpr Long select(ITL_EntryN<T, N>) noexcept { return (Long) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TL_
	: private	 ITL_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = ITL_Impl<IndexSequenceFor<Ts...>, Ts...>;

	static constexpr Long SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr Long index() noexcept {
		return Base::template select<T>(TL_{});
	}
};

template <typename TList, typename T>
constexpr Long index   () noexcept { return TList::template index   <T>(); }

template <typename TList, typename T>
constexpr bool contains() noexcept { return std::is_base_of<ITL_EntryT<T>, TList>::value; }

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TL_<Ts...>> {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using PrependTypes = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <typename...>
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
	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependTypes<TFirst, LowerTypes<NHalf, NIndex + 1, TRest...>>,
					 LowerTypes<NHalf, NIndex + 1, TRest...>
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
				 >::type;
};

template <Long NHalf, Long NIndex>
struct UpperT<NHalf, NIndex> {
	using Type = TL_<>;
};

template <typename... Ts>
using RHalfTypes = UpperTypes<sizeof...(Ts) / 2, 0, Ts...>;

////////////////////////////////////////////////////////////////////////////////

#else

template <typename>
struct Type {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TL_
	: Type<Ts>...
{
	static constexpr Long SIZE = sizeof...(Ts);
};

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

#endif

////////////////////////////////////////////////////////////////////////////////

}

template <typename TList, typename T>
constexpr Long index   () noexcept { return detail::Find<TList, T>::VALUE;					}

template <typename TList, typename T>
constexpr bool contains() noexcept { return std::is_base_of<detail::Type<T>, TList>::value;	}

//------------------------------------------------------------------------------

}
#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename...>
using Void = void;

//------------------------------------------------------------------------------

template <typename TKey, typename TValue>
struct TP_ {};

//------------------------------------------------------------------------------

template <typename...>
struct TT_;

template <typename... TKeys, typename... TValues>
struct TT_<TP_<TKeys, TValues>...>
	: TP_<TKeys, TValues>...
{};

//------------------------------------------------------------------------------

template <typename TKey, typename TValue>
constexpr TValue
tableValue(TP_<TKey, TValue>) noexcept { return TValue{}; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename TTypeTable, typename TKey, typename = void>
struct MatchT {
	using Type = void;
};

template<typename TTypeTable, typename TKey>
struct MatchT<TTypeTable, TKey, Void<decltype(tableValue<TKey>(TTypeTable{}))>> {
	using Type = decltype(tableValue<TKey>(TTypeTable{}));
};

template<typename TTypeTable, typename TKey>
using MatchType = typename MatchT<TTypeTable, TKey>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename, typename>
struct MatchListT;

template <typename... TKeys, typename... TValues, typename... Ts>
struct MatchListT<TT_<TP_<TKeys, TValues>...>, TL_<Ts...>> {
	using Type = TL_<MatchType<TT_<TP_<TKeys, TValues>...>, Ts>...>;
};

template <typename TTable, typename TList>
using MatchTypes = typename MatchListT<TTable, TList>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long... Ns>
struct VL_ {
	static constexpr Long SIZE = sizeof...(Ns);
};

//------------------------------------------------------------------------------

template <Long, typename>
struct PrependN;

template <Long N, Long... Ns>
struct PrependN<N, VL_<Ns...>> {
	using Type = VL_<N, Ns...>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long N, typename VL>
using PrependValues = typename PrependN<N, VL>::Type;

//------------------------------------------------------------------------------

template <Long, Long, Long...>
struct LowerN;

template <Long NHalf, Long NIndex, Long... Ns>
using LowerValues = typename LowerN<NHalf, NIndex, Ns...>::Type;

template <Long NHalf, Long NIndex, Long NFirst, Long... NRest>
struct LowerN<NHalf, NIndex, NFirst, NRest...> {
	using LValueList = typename LowerN<NHalf, NIndex + 1, NRest...>::Type;

	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependValues<NFirst, LValueList>,
					 LValueList
				 >;
};

template <Long NHalf, Long NIndex>
struct LowerN<NHalf, NIndex> {
	using Type = VL_<>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long... Ns>
using LHalfValues = LowerValues<sizeof...(Ns) / 2, 0, Ns...>;

//------------------------------------------------------------------------------

template <Long, Long, Long...>
struct UpperN;

template <Long NHalf, Long NIndex, Long... Ns>
using UpperValues = typename UpperN<NHalf, NIndex, Ns...>::Type;

template <Long NHalf, Long NIndex, Long NFirst, Long... NRest>
struct UpperN<NHalf, NIndex, NFirst, NRest...> {
	using Type = Conditional<
					 (NIndex < NHalf),
					 UpperValues<NHalf, NIndex + 1, NRest...>,
					 VL_<NFirst, NRest...>
				 >;
};

template <Long NHalf, Long NIndex>
struct UpperN<NHalf, NIndex> {
	using Type = VL_<>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long... Ns>
using RHalfValues = UpperValues<sizeof...(Ns) / 2, 0, Ns...>;

//------------------------------------------------------------------------------

template <Long NFirst, typename>
struct BinaryLookupT;

template <Long NFirst, Long... Ns>
struct BinaryLookupT<NFirst, VL_<Ns...>> {
	static constexpr Long FIRST	= NFirst;

	using LHalfList = LHalfValues<Ns...>;
	using RHalfList = RHalfValues<Ns...>;

	using LHalf = BinaryLookupT<FIRST				   , LHalfList>;
	using RHalf = BinaryLookupT<FIRST + LHalfList::SIZE, RHalfList>;

	static constexpr Long get(const long index) {
		return index < FIRST + LHalfList::SIZE ?
			LHalf::get(index) : RHalf::get(index);
	}
};

template <Long NFirst, Long N>
struct BinaryLookupT<NFirst, VL_<N>> {
	static constexpr Long get(const long /*index*/) { return N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TValueList>
using BinaryLookup = BinaryLookupT<0, TValueList>;

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NKey, Long NValue>
struct VP_ {};

//------------------------------------------------------------------------------

template <typename...>
struct VT_;

template <Long... NKeys, Long... NValues>
struct VT_<VP_<NKeys, NValues>...>
	: VP_<NKeys, NValues>...
{};

//------------------------------------------------------------------------------

template <Long NKey, Long NValue>
constexpr Long
matchValue(VP_<NKey, NValue>) noexcept { return NValue; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename TValueTable, Long NKey, typename = void>
struct MatchValueN {
	static constexpr Long VALUE = INVALID_LONG;
};

template<typename TValueTable, Long NKey>
struct MatchValueN<TValueTable, NKey, Void<decltype(matchValue<NKey>(TValueTable{}))>> {
	static constexpr Long VALUE = matchValue<NKey>(TValueTable{});
};

//------------------------------------------------------------------------------

template <typename, typename>
struct MatchValuesN;

template <typename... TKeys, typename... TValues, typename... Ts>
struct MatchValuesN<TT_<TP_<TKeys, TValues>...>, TL_<Ts...>> {
	using TypeList = TL_<Ts...>;

	using ValueTable = VT_<
						   VP_<
							   index<TypeList, TKeys>(),
							   index<TypeList, TValues>()
						   >...
					   >;

	using Type = VL_<
		MatchValueN<
			ValueTable,
			index<TypeList, Ts>()
		>::VALUE...
	>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TTable, typename TList>
using MatchValues = typename MatchValuesN<TTable, TList>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

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

#ifdef FFSM2_ENABLE_ANY_PLANS
	PLAN_SUCCEEDED,
	PLAN_FAILED,
#endif

	COUNT
};

#ifdef FFSM2_ENABLE_ANY_PLANS

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

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

#ifdef FFSM2_ENABLE_ANY_PLANS
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
#endif

	default:
		FFSM2_BREAK();
		return nullptr;
	}
}

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

	FFSM2_INLINE TransitionBase(const StateID destination_) noexcept
		: destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionBase(const StateID origin_,
								const StateID destination_) noexcept
		: origin	 {origin_}
		, destination{destination_}
	{}

	//----------------------------------------------------------------------

	FFSM2_INLINE bool operator == (const TransitionBase& other) const noexcept {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	//----------------------------------------------------------------------

	FFSM2_INLINE void clear() noexcept {
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

	//FFSM2_INLINE TransitionT() noexcept = default;

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

#ifdef FFSM2_ENABLE_ANY_PLANS
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

#ifdef FFSM2_ENABLE_ANY_PLANS

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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

struct TaskBase {
	FFSM2_INLINE TaskBase() noexcept {}

	FFSM2_INLINE TaskBase(const StateID origin_,
						  const StateID destination_) noexcept
		: origin{origin_}
		, destination{destination_}
	{}

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
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

	bool payloadSet = false;
	Storage storage;
};

//------------------------------------------------------------------------------

template <>
struct TaskT<void>
	: TaskBase
{
	using TaskBase::TaskBase;
};

//------------------------------------------------------------------------------

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
		, Long
		, Long
		FFSM2_IF_STATIC_PLANS(, typename)
		, typename>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
					 FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
					 , TPayload>>
{
	using StateList		= TStateList;
	using Payload		= TPayload;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<TPayload>;
	using Tasks			= List<Task, TASK_CAPACITY>;
	using TaskLinks		= StaticArray<TaskLink, TASK_CAPACITY>;
	using Payloads		= StaticArray<Payload, TASK_CAPACITY>;

	using TasksBits		= BitArray<StateID, StateList::SIZE>;

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
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
					 FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
					 , void>>
{
	using StateList		= TStateList;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= List<Task, TASK_CAPACITY>;
	using TaskLinks		= StaticArray<TaskLink, TASK_CAPACITY>;

	using TasksBits		= BitArray<StateID, StateList::SIZE>;

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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef FFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
Long
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::verifyPlan() const noexcept {
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

					if (fast != INVALID_LONG) {
						fast = taskLinks[fast].next;
					}

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

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), void>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef FFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), void>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL)>
Long
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), void>>::verifyPlan() const noexcept {
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

					if (fast != INVALID_LONG) {
						fast = taskLinks[fast].next;
					}

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

	inline explicit operator bool() const noexcept		{ return result != Result::NONE;	}

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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

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

	struct Iterator {
		FFSM2_INLINE Iterator(const CPlanT& plan)		noexcept;

		FFSM2_INLINE explicit operator bool() const		noexcept;

		FFSM2_INLINE void operator ++()					noexcept;

		FFSM2_INLINE const Task& operator  *() const	noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const	noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next() const					noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_INLINE CPlanT(const PlanData& planData)		noexcept;

	template <typename T>
	static constexpr StateID  stateId()					noexcept	{ return			index<StateList , T>();	}

public:
	FFSM2_INLINE explicit operator bool() const			noexcept;

	FFSM2_INLINE Iterator first()						noexcept	{ return Iterator{*this};					}

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

	struct Iterator {
		FFSM2_INLINE Iterator(PlanBaseT& plan)			noexcept;

		FFSM2_INLINE explicit operator bool() const		noexcept;

		FFSM2_INLINE void operator ++()					noexcept;

		FFSM2_INLINE	   Task& operator  *()			noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *() const	noexcept	{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE	   Task* operator ->()			noexcept	{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const	noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE void remove()						noexcept;

		FFSM2_INLINE Long next() const					noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator {
		FFSM2_INLINE CIterator(const PlanBaseT& plan)	noexcept;

		FFSM2_INLINE explicit operator bool() const		noexcept;

		FFSM2_INLINE void operator ++()					noexcept;

		FFSM2_INLINE	   Task& operator  *()			noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *() const	noexcept	{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE	   Task* operator ->()			noexcept	{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const	noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next() const					noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_INLINE PlanBaseT(PlanData& planData)			noexcept;

	template <typename T>
	static constexpr StateID  stateId()					noexcept	{ return			index<StateList , T>();	}

	FFSM2_INLINE bool append(const StateID origin,
							 const StateID destination)	noexcept;

	FFSM2_INLINE bool linkTask(const Long index)		noexcept;

public:
	FFSM2_INLINE explicit operator bool() const			noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_INLINE void clear()							noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool change(const StateID origin, const StateID destination)	noexcept	{ return append(origin, destination);							}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool change(const StateID destination)							noexcept	{ return change(stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool change()													noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	FFSM2_INLINE  Iterator first()												noexcept	{ return  Iterator{*this};										}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_INLINE CIterator first() const										noexcept	{ return CIterator{*this};										}

private:
	void remove(const Long task)												noexcept;

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
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)
		, typename TPayload>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, NSubstitutionLimit
				, NTaskCapacity
				FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
				, TPayload>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , NSubstitutionLimit
						   , NTaskCapacity
						   FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
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
					 , NSubstitutionLimit
					 , NTaskCapacity
					 FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
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
	FFSM2_INLINE bool changeWith(const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append	   (origin								 , destination								 ,  		 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith(const StateID origin, const StateID destination,	   Payload&& payload) noexcept	{ return append	   (origin								 , destination								 , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith(					   const StateID destination, const Payload& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), destination								 , 			 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith(					   const StateID destination,	   Payload&& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), destination								 , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith(												  const Payload& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), 			 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith(													   Payload&& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, NSubstitutionLimit
				, NTaskCapacity
				FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
				, void>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , NSubstitutionLimit
						   , NTaskCapacity
						   FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
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
					 , NSubstitutionLimit
					 , NTaskCapacity
					 FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

template <typename TArgs>
CPlanT<TArgs>::Iterator::Iterator(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
CPlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
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
PlanBaseT<TArgs>::Iterator::Iterator(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::Iterator::remove() noexcept {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
PlanBaseT<TArgs>::Iterator::next() const noexcept {
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

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
bool
PlanT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::append(const StateID origin,
																			  const StateID destination,
																			  const Payload& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, payload));
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
bool
PlanT<ArgsT<TC, TG, TSL, NSL, NTC FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::append(const StateID origin,
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
		, Long
		FFSM2_IF_DYNAMIC_PLANS(, Long)
		FFSM2_IF_STATIC_PLANS(, typename)
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry {
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

	template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSets	= Array<Transition, TArgs::SUBSTITUTION_LIMIT>;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using PlanData			= PlanDataT<TArgs>;
	using CPlan				= CPlanT<TArgs>;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		FFSM2_INLINE Origin(ControlT& control_,
							const StateID stateId) noexcept;

		FFSM2_INLINE ~Origin() noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE ControlT(Context& context
						, Registry& registry
						, Transition& request
						FFSM2_IF_DYNAMIC_PLANS(, PlanData& planData)
						FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: _context{context}
		, _registry{registry}
		, _request{request}
		FFSM2_IF_DYNAMIC_PLANS(, _planData{planData})
		FFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()			   noexcept	{ return index<StateList, TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_INLINE	   Context& _()				   noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_INLINE const Context& _()			 const noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_INLINE	   Context& context()		   noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_INLINE const Context& context()	 const noexcept	{ return _context;						}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	FFSM2_INLINE const Transition& request() const noexcept	{ return _request;						}

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

	/// @brief Access read-only plan
	/// @return Plan
	FFSM2_INLINE CPlan plan()				 const noexcept	{ return CPlan{_planData};				}

#endif

	//----------------------------------------------------------------------

protected:
#ifdef FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_INLINE Logger* logger()				   noexcept	{ return _logger;						}
#endif

protected:
	Context& _context;
	Registry& _registry;
	Transition& _request;
	StateID _originId = INVALID_STATE_ID;
	FFSM2_IF_DYNAMIC_PLANS(PlanData& _planData);
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

	template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
	friend class R_;

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using typename Control::PlanData;
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;
#endif

	using Control::Control;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

	/// @brief Access plan
	/// @return Plan
	FFSM2_INLINE  Plan plan()						noexcept	{ return  Plan{_planData};				}

	/// @brief Access read-only plan
	/// @return Read-only plan
	FFSM2_INLINE CPlan plan() const					noexcept	{ return CPlan{_planData};				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_IF_DYNAMIC_PLANS(using Control::_planData);
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

	template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
	friend class R_;

protected:
	using PlanControl		= PlanControlT<TArgs>;

	using typename PlanControl::StateList;

	using typename PlanControl::Transition;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using TasksBits			= BitArray<StateID, StateList::SIZE>;
#endif

#ifdef FFSM2_ENABLE_STATIC_PLANS
	using LinksTypeTable	= typename TArgs::LinksTypeTable;
	using LinksStateIDs		= MatchValues<LinksTypeTable, StateList>;
	using LinksLookup		= BinaryLookup<LinksStateIDs>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		FFSM2_INLINE Lock(FullControlBaseT& control_) noexcept;
		FFSM2_INLINE ~Lock() noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// Clang trips over 'stateId<>()', so give it a hint it comes from PlanControl

	/// @brief Transition into a state
	/// @param stateId State identifier
	FFSM2_INLINE void changeTo(const StateID stateId) noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_INLINE void changeTo() noexcept		{ changeTo (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ANY_PLANS

	/// @brief Succeed a plan task for the current state
	FFSM2_INLINE void succeed() noexcept;

	/// @brief Fail a plan task for the current state
	FFSM2_INLINE void fail()	noexcept;

#endif

	//----------------------------------------------------------------------

protected:
	using PlanControl::_request;
	FFSM2_IF_DYNAMIC_PLANS(using PlanControl::_planData);
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
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity)
					   FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , NSubstitutionLimit
								  FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity)
								  FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
								  , TPayload>>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity)
					 FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
					 , TPayload>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

#ifdef FFSM2_ENABLE_STATIC_PLANS
	using LinksLookup	= typename FullControlBase::LinksLookup;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef FFSM2_ENABLE_ANY_PLANS

	template <typename TState>
	void updatePlan(TState& headState, const Status subStatus) noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::changeTo;

	FFSM2_IF_DYNAMIC_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
								 const Payload& payload) noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
									  Payload&& payload) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(const Payload& payload) noexcept	{ changeWith(FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(	  Payload&& payload) noexcept	{ changeWith(FullControlBase::template stateId<TState>(), std::move(payload));	}

	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_request;
	FFSM2_IF_DYNAMIC_PLANS(using FullControlBase::_planData);
	FFSM2_IF_ANY_PLANS(using FullControlBase::_registry);
	FFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity)
					   FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , NSubstitutionLimit
								  FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity)
								  FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
								  , void>>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity)
					 FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
					 , void>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

#ifdef FFSM2_ENABLE_STATIC_PLANS
	using LinksLookup	= typename FullControlBase::LinksLookup;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef FFSM2_ENABLE_ANY_PLANS

	template <typename TState>
	void updatePlan(TState& headState, const Status subStatus) noexcept;

#endif

public:
	using FullControlBase::changeTo;

	FFSM2_IF_DYNAMIC_PLANS(using FullControlBase::plan);

protected:
	FFSM2_IF_DYNAMIC_PLANS(using FullControlBase::_planData);
	FFSM2_IF_ANY_PLANS(using FullControlBase::_registry);

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

	template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Context;

	using typename FullControl::Transition;
	using typename FullControl::TransitionSets;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using typename FullControl::PlanData;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE GuardControlT(Context& context
						  , Registry& registry
						  , Transition& request
						  , const TransitionSets& currentTransitions
						  , const Transition& pendingTransition
						  FFSM2_IF_DYNAMIC_PLANS(, PlanData& planData)
						  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, request
					FFSM2_IF_DYNAMIC_PLANS(, planData)
					FFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransitions{currentTransitions}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE const TransitionSets& currentTransitions() const	noexcept	{ return _currentTransitions;	}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	FFSM2_INLINE Transition& pendingTransition() const				noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	FFSM2_INLINE void cancelPendingTransition()						noexcept;

private:
	using FullControl::_registry;
	using FullControl::_originId;

	FFSM2_IF_LOG_INTERFACE(using FullControl::_logger);

	const TransitionSets& _currentTransitions;
	const Transition& _pendingTransition;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

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

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ANY_PLANS

template <typename TArgs>
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	FFSM2_IF_DYNAMIC_PLANS(_planData.tasksSuccesses.set(_originId));

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	FFSM2_IF_DYNAMIC_PLANS(_planData.tasksFailures.set(_originId));

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_ANY_PLANS

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
template <typename TState>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::updatePlan(TState& FFSM2_IF_DYNAMIC_PLANS(headState),
																												  const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}
	#endif

	} else if (subStatus.result == Status::Result::SUCCESS) {
		FFSM2_IF_STATIC_PLANS(bool processed = false);

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it)
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					it.remove();

					FFSM2_IF_STATIC_PLANS(processed = true);
					break;
				}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}

	#endif

	#ifdef FFSM2_ENABLE_STATIC_PLANS

		if (!processed) {
			const StateID destination = LinksLookup::get(_registry.active);

			if (destination != INVALID_STATE_ID) {
				Origin origin{*this, _registry.active};
				changeTo(destination);
			}
		}

	#endif
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::changeWith(const StateID  stateId,
																												  const Payload& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::changeWith(const StateID  stateId,
																													   Payload&& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_ANY_PLANS

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL)>
template <typename TState>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), void>>::updatePlan(TState& FFSM2_IF_DYNAMIC_PLANS(headState),
																												   const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}

	#endif

	} else if (subStatus.result == Status::Result::SUCCESS) {
		FFSM2_IF_STATIC_PLANS(bool processed = false);

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it)
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					it.remove();

					FFSM2_IF_STATIC_PLANS(processed = true);
					break;
				}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}

	#endif

	#ifdef FFSM2_ENABLE_STATIC_PLANS

		if (!processed) {
			const StateID destination = LinksLookup::get(_registry.active);

			if (destination != INVALID_STATE_ID) {
				Origin origin{*this, _registry.active};
				changeTo(destination);
			}
		}

	#endif
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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	FFSM2_INLINE void preEntryGuard(Context&) noexcept {}

	FFSM2_INLINE void preEnter	   (Context&) noexcept {}
	FFSM2_INLINE void preReenter   (Context&) noexcept {}

	FFSM2_INLINE void preUpdate	   (Context&) noexcept {}

	template <typename TEvent>
	FFSM2_INLINE void preReact	   (const TEvent&,
									Context&) noexcept {}

	FFSM2_INLINE void preExitGuard (Context&) noexcept {}

	FFSM2_INLINE void postExit	   (Context&) noexcept {}

	template <typename T>
	static constexpr StateID  stateId() noexcept { return index<StateList, T>();	}
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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void widePreEntryGuard(Context& context) noexcept;

	FFSM2_INLINE void widePreEnter	   (Context& context) noexcept;
	FFSM2_INLINE void widePreReenter   (Context& context) noexcept;

	FFSM2_INLINE void widePreUpdate	   (Context& context) noexcept;

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context) noexcept;

	FFSM2_INLINE void widePreExitGuard (Context& context) noexcept;

	FFSM2_INLINE void widePostExit	   (Context& context) noexcept;
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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void entryGuard	   (GuardControl&)		noexcept {}

	FFSM2_INLINE void enter			   (PlanControl&)		noexcept {}
	FFSM2_INLINE void reenter		   (PlanControl&)		noexcept {}

	FFSM2_INLINE void update		   (FullControl&)		noexcept {}

	template <typename TEvent>
	FFSM2_INLINE void react			   (const TEvent&,
										FullControl&)		noexcept {}

	FFSM2_INLINE void exitGuard		   (GuardControl&)		noexcept {}

	FFSM2_INLINE void exit			   (PlanControl&)		noexcept {}

#ifdef FFSM2_ENABLE_ANY_PLANS
	FFSM2_INLINE void planSucceeded	   (FullControl&)		noexcept {}
	FFSM2_INLINE void planFailed	   (FullControl&)		noexcept {}
#endif

	FFSM2_INLINE void widePreEntryGuard(Context& context)	noexcept;

	FFSM2_INLINE void widePreEnter	   (Context& context)	noexcept;
	FFSM2_INLINE void widePreReenter   (Context& context)	noexcept;

	FFSM2_INLINE void widePreUpdate	   (Context& context)	noexcept;

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context)	noexcept;

	FFSM2_INLINE void widePreExitGuard (Context& context)	noexcept;

	FFSM2_INLINE void widePostExit	   (Context& context)	noexcept;
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

	static constexpr bool isBare() noexcept							{ return false;						}

	union {
		Type t_;
	};

	FFSM2_INLINE  DynamicBox() noexcept {}
	FFSM2_INLINE ~DynamicBox() noexcept {}

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control) noexcept	{ Guard<Type>::execute(control);	}

	FFSM2_INLINE void construct() noexcept;
	FFSM2_INLINE void  destruct() noexcept;

	FFSM2_INLINE	   Type& get()		 noexcept			{ FFSM2_ASSERT(initialized_); return t_;	}
	FFSM2_INLINE const Type& get() const noexcept			{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare() noexcept	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control) noexcept;

	FFSM2_INLINE void construct() noexcept 																{}
	FFSM2_INLINE void  destruct() noexcept 																{}

	FFSM2_INLINE	   Type& get()		 noexcept					{ return t_;						}
	FFSM2_INLINE const Type& get() const noexcept					{ return t_;						}

	FFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Type));
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

#ifdef FFSM2_ENABLE_ANY_PLANS
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
	FFSM2_IF_DYNAMIC_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

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
	FFSM2_IF_DYNAMIC_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	control._planData.clearTaskStatus(STATE_ID);
#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ANY_PLANS

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

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)
		, typename TPayload>
struct ArgsT final {
	using Context			= TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TConfig::LoggerInterface;
#endif

	using StateList			= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

#ifdef FFSM2_ENABLE_STATIC_PLANS
	using LinksTypeTable	= TLinksTypeTable;
#endif

	using Payload			= TPayload;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <StateID, typename, Short, typename...>
struct CS_;

template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
class RW_;

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

template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
struct RF_ {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT = TConfig::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	static constexpr Long  TASK_CAPACITY	  = TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload		= typename TConfig::Payload;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using Task			= typename TConfig::Task;
#endif

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , SUBSTITUTION_LIMIT
							  FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY)
							  FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<TConfig, Apex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

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
		FFSM2_IF_ANY_PLANS(const Status subStatus =)
		_subStates.wideUpdate(control, active);

	#ifdef FFSM2_ENABLE_ANY_PLANS
		if (subStatus FFSM2_IF_DYNAMIC_PLANS(&& control._planData.planExists))
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
		FFSM2_IF_ANY_PLANS(const Status subStatus =)
		_subStates.wideReact(control, event, active);

	#ifdef FFSM2_ENABLE_ANY_PLANS
		if (subStatus FFSM2_IF_DYNAMIC_PLANS(&& control._planData.planExists))
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

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	auto plan = control.plan();
	plan.clear();
#endif
}

//------------------------------------------------------------------------------

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
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct G_ {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	static constexpr Long TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , SUBSTITUTION_LIMIT FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, N				   FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY), Payload>;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT				  , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY), T      >;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>> {
	using Cfg = G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	/// @brief Topology An optional wrapper
	///		should be passed in conjunction with StaticPlan
	/// @tparam TSubStates Sub-states
	template <typename... TSubStates>
	struct Topology;

#ifdef FFSM2_ENABLE_STATIC_PLANS

	/// @brief An entry in a transition table
	/// @tparam TOrigin Origin state
	/// @tparam TDestination Destination state
	template <typename TOrigin, typename TDestination>
	using TaskLink = TP_<TOrigin, TDestination>;

	/// <summary>
	///
	/// </summary>
	/// <typeparam name="...TLinks"></typeparam>
	template <typename... TLinks>
	using StaticPlan = TT_<TLinks...>;

#endif

	//----------------------------------------------------------------------

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	struct Root final
		: RF_<Cfg,	 CI_<THead, TSubStates...> FFSM2_IF_STATIC_PLANS(, TT_<>)>
	{};

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	struct Root<THead, Topology<TSubStates...>> final
		: RF_<Cfg,	 CI_<THead, TSubStates...> FFSM2_IF_STATIC_PLANS(, TT_<>)>
	{};

#ifdef FFSM2_ENABLE_STATIC_PLANS

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam Topology A list of sub-states
	/// @tparam StaticPlan Static plan
	template <typename THead, typename... TSubStates, typename... TOrigins, typename... TDestinations>
	struct Root<THead, Topology<TSubStates...>, TT_<TP_<TOrigins, TDestinations>...>> final
		: RF_<Cfg,	 CI_<THead, TSubStates...>, TT_<TP_<TOrigins, TDestinations>...>>
	{};

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Headless root
	/// @tparam TSubStates Sub-states
	template <	 typename... TSubStates>
	struct PeerRoot final
		: RF_<Cfg, CI_<void, TSubStates...> FFSM2_IF_STATIC_PLANS(, TT_<>)>
	{};

	/// @brief Headless root
	/// @tparam Topology A list of sub-states
	template <	 typename... TSubStates>
	struct PeerRoot<Topology<TSubStates...>> final
		: RF_<Cfg, CI_<void, TSubStates...> FFSM2_IF_STATIC_PLANS(, TT_<>)>
	{};

#ifdef FFSM2_ENABLE_STATIC_PLANS

	/// @brief Headless root
	/// @tparam Topology A list of sub-states
	template <	 typename... TSubStates, typename... TOrigins, typename... TDestinations>
	struct PeerRoot<Topology<TSubStates...>, TT_<TP_<TOrigins, TDestinations>...>> final
		: RF_<Cfg, CI_<void, TSubStates...>, TT_<TP_<TOrigins, TDestinations>...>>
	{};

#endif

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

/// @brief Type configuration for MachineT<>
/// @tparam TContext Context type for data shared between states and/or data interface between FSM and external code
/// @tparam NSubstitutionLimit Maximum number times 'guard()' methods can substitute their states for others
/// @tparam NTaskCapacity Maximum number of tasks across all plans
/// @tparam TPayload Payload type
template <typename TContext = EmptyContext
		, Long NSubstitutionLimit = 4
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity = INVALID_LONG)
		, typename TPayload = void>
using ConfigT = detail::G_<FFSM2_FEATURE_TAG, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>;

/// @brief Type configuration for MachineT<>
using Config = ConfigT<>;

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
template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class R_ {
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

	using Payload				= typename TConfig::Payload;

	using Forward				= RF_<TConfig, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;
	using StateList				= typename Forward::StateList;
	using Args					= typename Forward::Args;

	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	using MaterialApex			= Material<0, Args, TApex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using PlanData				= PlanDataT<Args>;

	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

public:
	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSets		= typename Control::TransitionSets;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	FFSM2_INLINE explicit R_(Context& context
						   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	~R_() noexcept;

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()						noexcept	{ return index<StateList, TState>();			}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update() noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_INLINE void react(const TEvent& event)			noexcept;

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_INLINE StateID activeStateId()			  const	noexcept	{ return _registry.active;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	FFSM2_INLINE bool isActive(const StateID stateId) const	noexcept	{ return _registry.active == stateId;			}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_INLINE bool isActive()					  const	noexcept	{ return _registry.active == stateId<TState>();	}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	FFSM2_INLINE void changeTo(const StateID stateId)		noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_INLINE void changeTo()							noexcept	{ changeTo (stateId<TState>());					}

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	inline void attachLogger(Logger* const logger)			noexcept	{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter() noexcept;

	void processTransitions(TransitionSets& currentTransitions) noexcept;

	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
								const Transition& pendingTransition) noexcept;

	bool cancelledByGuards(const TransitionSets& currentTransitions,
						   const Transition& pendingTransition) noexcept;

protected:
	Context& _context;

	Registry _registry;
	FFSM2_IF_DYNAMIC_PLANS(PlanData _planData);

	Transition _request;

	MaterialApex _apex;

	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

	using Payload				= TPayload;
	using Transition			= TransitionT<Payload>;

public:
	using Base::Base;

	using Base::stateId;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()					 noexcept	{ return Base::template stateId<TState>();				}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
								 const Payload& payload) noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
									  Payload&& payload) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(const Payload& payload) noexcept	{ changeWith(stateId<TState>(),			  payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(	  Payload&& payload) noexcept	{ changeWith(stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using Base::_context;
	using Base::_request;

	FFSM2_IF_LOG_INTERFACE(using Base::_logger);
};

//------------------------------------------------------------------------------

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), void>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), void>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), void>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RW_ final
	: public	 RP_<TConfig, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
{
	using Base = RP_<TConfig, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

public:
	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == ::ffsm2::EmptyContext

template <
		  Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RW_		<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)> final
	: public RP_<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
	, ::ffsm2::EmptyContext
{
	using Cfg =  ::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>;

	using Context	= typename Cfg::Context;
	using Base		= RP_<Cfg, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:
	explicit FFSM2_INLINE RW_(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<Context&>(*this)
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::R_(Context& context
										  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::~R_() noexcept {
	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_DYNAMIC_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	FFSM2_IF_DYNAMIC_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::update() noexcept {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_DYNAMIC_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	FFSM2_IF_DYNAMIC_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
template <typename TEvent>
void
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::react(const TEvent& event) noexcept {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_DYNAMIC_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::changeTo(const StateID stateId) noexcept {
	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::initialEnter() noexcept {
	FFSM2_ASSERT(_request.destination == INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_DYNAMIC_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_registry.requested = 0;

	TransitionSets currentTransitions;
	Transition pendingTransition;

	cancelledByEntryGuards(currentTransitions,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _request.destination != INVALID_SHORT;
		 ++i)
	{
		_registry.requested = _request.destination;
		pendingTransition = _request;
		_request.clear();

		if (cancelledByEntryGuards(currentTransitions,
								   pendingTransition))
			FFSM2_BREAK();
		else
			currentTransitions += pendingTransition;

		pendingTransition.clear();
	}

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);

	_registry.clearRequests();

	FFSM2_IF_DYNAMIC_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::processTransitions(TransitionSets& currentTransitions) noexcept {
	FFSM2_ASSERT(_request.destination != INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_DYNAMIC_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Transition pendingTransition;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _request.destination != INVALID_SHORT;
		++i)
	{
		_registry.requested = _request.destination;
		pendingTransition = _request;
		_request.clear();

		if (cancelledByGuards(currentTransitions, pendingTransition))
			;
		else
			currentTransitions += pendingTransition;

		pendingTransition.clear();
	}

	if (currentTransitions.count())
		_apex.deepChangeToRequested(control);

	_registry.clearRequests();

	FFSM2_IF_DYNAMIC_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
bool
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
																const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_DYNAMIC_PLANS(, _planData)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
bool
R_<TG, TA FFSM2_IF_STATIC_PLANS(, TPL)>::cancelledByGuards(const TransitionSets& currentTransitions,
														   const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_DYNAMIC_PLANS(, _planData)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC), typename TP, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
RP_<G_<NFT, TC, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC), TP>, TA FFSM2_IF_STATIC_PLANS(, TPL)>::changeWith(const StateID  stateId,
																									 const Payload& payload) noexcept
{
	_request = Transition{stateId, payload};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC), typename TP, typename TA FFSM2_IF_STATIC_PLANS(, typename TPL)>
void
RP_<G_<NFT, TC, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC), TP>, TA FFSM2_IF_STATIC_PLANS(, TPL)>::changeWith(const StateID  stateId,
																										  Payload&& payload) noexcept
{
	_request = Transition{stateId, std::move(payload)};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

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

#undef FFSM2_IF_DYNAMIC_PLANS
#undef FFSM2_IF_STATIC_PLANS
#undef FFSM2_IF_DYNAMIC_PLANS
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
