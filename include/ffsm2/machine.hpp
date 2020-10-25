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
	#define FFSM2_IF_PLANS(...)										  __VA_ARGS__
	#define FFSM2_PLANS_MASK											 (1 << 0)
#else
	#define FFSM2_IF_PLANS(...)
	#define FFSM2_PLANS_MASK											 (0 << 0)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_SERIALIZATION
	#define FFSM2_IF_SERIALIZATION(...)								  __VA_ARGS__
	#define FFSM2_SERIALIZATION_MASK									 (1 << 1)
#else
	#define FFSM2_IF_SERIALIZATION(...)
	#define FFSM2_SERIALIZATION_MASK									 (0 << 1)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_TRANSITION_HISTORY
	#define FFSM2_IF_TRANSITION_HISTORY(...)						  __VA_ARGS__
	#define FFSM2_TRANSITION_HISTORY_MASK								 (1 << 2)
#else
	#define FFSM2_IF_TRANSITION_HISTORY(...)
	#define FFSM2_TRANSITION_HISTORY_MASK								 (0 << 2)
#endif

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_STRUCTURE_REPORT
	#define FFSM2_IF_STRUCTURE_REPORT(...)							  __VA_ARGS__
	#define FFSM2_STRUCTURE_REPORT_MASK									 (1 << 3)
#else
	#define FFSM2_IF_STRUCTURE_REPORT(...)
	#define FFSM2_STRUCTURE_REPORT_MASK									 (0 << 3)
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define FFSM2_ENABLE_LOG_INTERFACE
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								 (1 << 4)
#else
	#define FFSM2_VERBOSE_DEBUG_LOG_MASK								 (0 << 4)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	#define FFSM2_IF_LOG_INTERFACE(...)								  __VA_ARGS__
	#define FFSM2_LOG_INTERFACE_MASK									 (1 << 5)

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
	#define FFSM2_LOG_INTERFACE_MASK									 (0 << 5)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_LOG_TASK_STATUS(CONTEXT, , ORIGIN, STATUS)
	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, , STATUS)
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

constexpr FeatureTag FFSM2_FEATURE_TAG = FFSM2_PLANS_MASK						|
										 FFSM2_SERIALIZATION_MASK				|
										 FFSM2_TRANSITION_HISTORY_MASK			|
										 FFSM2_STRUCTURE_REPORT_MASK			|
										 FFSM2_VERBOSE_DEBUG_LOG_MASK			|
										 FFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef FFSM2_PLANS_MASK
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

template <typename T>
FFSM2_INLINE
constexpr void
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

	using Type = typename std::conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 typename std::conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 typename std::conditional<CAPACITY <= UINT32_MAX, uint32_t,
																   uint64_t>::type>::type>::type;

	static_assert(CAPACITY <= UINT64_MAX, "STATIC ASSERT");
};

template <unsigned NCapacity>
using UnsignedCapacity = typename UnsignedCapacityT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <unsigned NBitWidth>
struct UnsignedBitWidthT {
	static constexpr Short BIT_WIDTH = NBitWidth;

	using Type = typename std::conditional<BIT_WIDTH <= 8,  uint8_t,
				 typename std::conditional<BIT_WIDTH <= 16, uint16_t,
				 typename std::conditional<BIT_WIDTH <= 32, uint32_t,
															uint64_t>::type>::type>::type;

	static_assert(BIT_WIDTH <= 64, "STATIC ASSERT");
};

template <unsigned NCapacity>
using UnsignedBitWidth = typename UnsignedBitWidthT<NCapacity>::Type;

//------------------------------------------------------------------------------

constexpr Long
roundUp(const Long x,
		const Long to)
{
	return (x + (to - 1)) / to;
}

//------------------------------------------------------------------------------

constexpr Short
bitWidth(const Short x) {
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
constexpr void
overwrite(TTo& to, const TFrom& from) {
	static_assert(sizeof(TTo) == sizeof(TFrom), "STATIC ASSERT");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TO, typename TI>
constexpr TO
convert(const TI& in) {
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
	constexpr Iterator(Container& container,
					   const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	constexpr bool operator != (const Iterator<Container>& dummy) const noexcept;

	constexpr Iterator& operator ++() noexcept;

	constexpr		Item& operator *()		 noexcept	{ return  _container[_cursor];	}
	constexpr const Item& operator *() const noexcept	{ return  _container[_cursor];	}

	constexpr		Item* operator->()		 noexcept	{ return &_container[_cursor];	}
	constexpr const Item* operator->() const noexcept	{ return &_container[_cursor];	}

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
	constexpr Iterator(const Container& container,
					   const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	constexpr bool operator != (const Iterator<const Container>& dummy) const noexcept;

	constexpr Iterator& operator ++() noexcept;

	constexpr const Item& operator *() const noexcept	{ return _container[_cursor];	}

	constexpr const Item* operator->() const noexcept	{ return &operator *();			}

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
constexpr bool
Iterator<T>::operator != (const Iterator<T>& FFSM2_IF_ASSERT(dummy)) const noexcept {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
constexpr Iterator<T>&
Iterator<T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
constexpr bool
Iterator<const T>::operator != (const Iterator<const T>& FFSM2_IF_ASSERT(dummy)) const noexcept {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
constexpr Iterator<const T>&
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
	constexpr StaticArray()									noexcept = default;
	constexpr StaticArray(const Item filler)				noexcept;

	template <typename N>
	constexpr		Item& operator[] (const N i)			noexcept;

	template <typename N>
	constexpr const Item& operator[] (const N i) const		noexcept;

	constexpr Long count() const							noexcept	{ return CAPACITY;									}

	constexpr void fill(const Item filler)					noexcept;
	constexpr void clear()									noexcept	{ fill(INVALID_SHORT);								}

	constexpr Iterator<      StaticArray>  begin()			noexcept	{ return Iterator<      StaticArray>(*this,     0); }
	constexpr Iterator<const StaticArray>  begin() const	noexcept	{ return Iterator<const StaticArray>(*this,     0); }
	constexpr Iterator<const StaticArray> cbegin() const	noexcept	{ return Iterator<const StaticArray>(*this,     0); }

	constexpr Iterator<      StaticArray>    end()			noexcept	{ return Iterator<      StaticArray>(*this, DUMMY);	}
	constexpr Iterator<const StaticArray>    end() const	noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}
	constexpr Iterator<const StaticArray>   cend() const	noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	constexpr StaticArray() = default;
	constexpr StaticArray(const Item)											{}
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
	constexpr void clear()										noexcept			{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	constexpr Long append(const TValue& value)					noexcept;

	template <typename TValue>
	constexpr Long append(	   TValue&& value)					noexcept;

	template <typename N>
	constexpr		Item& operator[] (const N i)				noexcept;

	template <typename N>
	constexpr const Item& operator[] (const N i) const			noexcept;

	constexpr Long count() const								noexcept	{ return _count;	}

	constexpr Array& operator += (const Item& item)				noexcept;
	constexpr Array& operator += (	   Item&& item)				noexcept;

	template <Long N>
	constexpr Array& operator += (const Array<Item, N>& other)	noexcept;

	constexpr Iterator<      Array>  begin()					noexcept	{ return Iterator<		Array>(*this,     0);	}
	constexpr Iterator<const Array>  begin() const				noexcept	{ return Iterator<const Array>(*this,     0);	}
	constexpr Iterator<const Array> cbegin() const				noexcept	{ return Iterator<const Array>(*this,     0);	}

	constexpr Iterator<      Array>	  end()						noexcept	{ return Iterator<		Array>(*this, DUMMY);	}
	constexpr Iterator<const Array>	  end()  const				noexcept	{ return Iterator<const Array>(*this, DUMMY);	}
	constexpr Iterator<const Array>   cend() const				noexcept	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	constexpr Long next(const Long i) const						noexcept	{ return i + 1;		}
	constexpr Long limit() const								noexcept	{ return _count;	}

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
constexpr
StaticArray<T, NC>::StaticArray(const Item filler) noexcept {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
constexpr T&
StaticArray<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
constexpr const T&
StaticArray<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
constexpr void
StaticArray<T, NC>::fill(const Item filler) noexcept {
	for (Long i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
constexpr Long
Array<T, NC>::append(const TValue& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
constexpr Long
Array<T, NC>::append(TValue&& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
constexpr T&
Array<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
constexpr const T&
Array<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
constexpr Array<T, NC>&
Array<T, NC>::operator += (const Item& item) noexcept {
	append(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
constexpr Array<T, NC>&
Array<T, NC>::operator += (Item&& item) noexcept {
	append(std::move(item));

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
constexpr Array<T, NC>&
Array<T, NC>::operator += (const Array<T, N>& other) noexcept {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	constexpr Units(Short unit_  = INVALID_SHORT,
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
		constexpr explicit Bits(Unit* const storage,
								const Index width) noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		constexpr explicit operator bool() const noexcept;

		constexpr void clear() noexcept;

		constexpr bool get  (const Index index) const noexcept;
		constexpr void set  (const Index index)		  noexcept;
		constexpr void clear(const Index index)		  noexcept;

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <typename, Short>
		friend class BitArray;

	private:
		constexpr explicit CBits(const Unit* const storage,
								 const Index width) noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		constexpr explicit operator bool() const noexcept;

		constexpr bool get(const Index index) const noexcept;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() {
		clear();
	}

	constexpr void clear() noexcept;

	constexpr bool get  (const Index index) const noexcept;
	constexpr void set  (const Index index)		  noexcept;
	constexpr void clear(const Index index)		  noexcept;

	template <Short NUnit, Short NWidth>
	constexpr  Bits bits()		 noexcept;

	template <Short NUnit, Short NWidth>
	constexpr CBits bits() const noexcept;

	constexpr  Bits bits(const Units& units)	   noexcept;
	constexpr CBits bits(const Units& units) const noexcept;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	constexpr void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
constexpr
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
constexpr void
BitArray<TI, NC>::Bits::clear() noexcept {
	const Index count = (_width + 7) / (sizeof(Unit) * 8);

	for (Index i = 0; i < count; ++i)
		_storage[i] = Unit{0};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
constexpr bool
BitArray<TI, NC>::Bits::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
constexpr void
BitArray<TI, NC>::Bits::set(const Index index) noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
constexpr void
BitArray<TI, NC>::Bits::clear(const Index index) noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
constexpr
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
constexpr bool
BitArray<TI, NC>::CBits::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
constexpr void
BitArray<TI, NC>::clear() noexcept {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
constexpr bool
BitArray<TI, NC>::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
constexpr void
BitArray<TI, NC>::set(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
constexpr void
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
constexpr typename BitArray<TI, NC>::Bits
BitArray<TI, NC>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
constexpr typename BitArray<TI, NC>::CBits
BitArray<TI, NC>::bits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
constexpr typename BitArray<TI, NC>::Bits
BitArray<TI, NC>::bits(const Units& units) noexcept {
	FFSM2_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
constexpr typename BitArray<TI, NC>::CBits
BitArray<TI, NC>::bits(const Units& units) const noexcept {
	FFSM2_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return CBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}
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

		constexpr Cell() noexcept
			: links{}
		{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	template <typename... TArgs>
	constexpr Index emplace(TArgs... args) noexcept;

	constexpr void remove(const Index i) noexcept;

	constexpr		Item& operator[] (const Index i)	   noexcept;
	constexpr const Item& operator[] (const Index i) const noexcept;

	constexpr Index count() const noexcept						{ return _count;	}

private:
	FFSM2_IF_ASSERT(constexpr void verifyStructure(const Index occupied = INVALID) const noexcept);

private:
	Cell _cells[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last = 0;
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
constexpr Long
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
constexpr void
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
constexpr T&
List<T, NC>::operator[] (const Index i) noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
constexpr const T&
List<T, NC>::operator[] (const Index i) const noexcept {
	FFSM2_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename T, Long NC>
constexpr void
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

////////////////////////////////////////////////////////////////////////////////

#if 0

namespace detail {

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
	static constexpr Long select(ITL_EntryN<T, N>) { return (Long) N; }
};

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TypeList
	: private	 detail::ITL_Impl<detail::IndexSequenceFor<Ts...>, Ts...>
{
	using Base = detail::ITL_Impl<detail::IndexSequenceFor<Ts...>, Ts...>;

	static constexpr Long SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr Long index() {
		return Base::template select<T>(TypeList{});
	}
};

template <typename TList, typename T>
constexpr Long index   () { return TList::template index   <T>(); }

template <typename TList, typename T>
constexpr bool contains() { return std::is_base_of<detail::ITL_EntryT<T>, TList>::value; }

////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TypeList<Ts...>> {
	using Type = TypeList<T, Ts...>;
};

}

template <typename... Ts>
using Prepend = typename detail::PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TypeList<Ts1...>, TypeList<Ts2...>> {
	using Type = TypeList<Ts1..., Ts2...>;
};

}

template <typename... Ts>
using Merge = typename detail::MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct LowerT;

template <Long H, Long I, typename... Ts>
using Lower = typename LowerT<H, I, Ts...>::Type;

template <Long H, Long I, typename TFirst, typename... TRest>
struct LowerT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<
					 (I < H),
					 Prepend<TFirst, Lower<H, I + 1, TRest...>>,
					 Lower<H, I + 1, TRest...>
				 >::type;
};

template <Long H, Long I>
struct LowerT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using LHalf = detail::Lower<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct UpperT;

template <Long H, Long I, typename... Ts>
using Upper = typename UpperT<H, I, Ts...>::Type;

template <Long H, Long I, typename TFirst, typename... TRest>
struct UpperT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<
					 (I < H),
					 UpperT<H, I + 1, TRest...>,
					 TypeList<TFirst, TRest...>
				 >::type;
};

template <Long H, Long I>
struct UpperT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using RHalf = detail::Upper<sizeof...(Ts) / 2, 0, Ts...>;

////////////////////////////////////////////////////////////////////////////////

#else

namespace detail {

template <typename>
struct Type {};

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TypeList
	: detail::Type<Ts>...
{
	static constexpr Long SIZE = sizeof...(Ts);
};

//------------------------------------------------------------------------------

namespace detail {

template <Long N>
struct Const {
	static constexpr Long Value = N;
};

//------------------------------------------------------------------------------

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TypeList<Ts...>> {
	using Type = TypeList<T, Ts...>;
};

}

template <typename... Ts>
using Prepend = typename detail::PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TypeList<Ts1...>, TypeList<Ts2...>> {
	using Type = TypeList<Ts1..., Ts2...>;
};

}

template <typename... Ts>
using Merge = typename detail::MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct LowerT;

template <Long H, Long I, typename... Ts>
using Lower = typename LowerT<H, I, Ts...>::Type;

template <Long H, Long I, typename TFirst, typename... TRest>
struct LowerT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<
					 (I < H),
					 Prepend<TFirst, Lower<H, I + 1, TRest...>>,
					 Lower<H, I + 1, TRest...>
				 >::type;
};

template <Long H, Long I>
struct LowerT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using LHalf = detail::Lower<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct UpperT;

template <Long H, Long I, typename... Ts>
using Upper = typename UpperT<H, I, Ts...>::Type;

template <Long H, Long I, typename TFirst, typename... TRest>
struct UpperT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<
					 (I < H),
					 Upper<H, I + 1, TRest...>,
					 TypeList<TFirst, TRest...>
				 >::type;
};

template <Long H, Long I>
struct UpperT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using RHalf = detail::Upper<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

namespace detail {

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
struct Find<T, TypeList<Ts...>>
	: FindImpl<0, T, Ts...>
{};

}

//------------------------------------------------------------------------------

template <typename TList, typename T>
constexpr Long index   () { return detail::Find<T, TList>::Value;					}

template <typename TList, typename T>
constexpr bool contains() { return std::is_base_of<detail::Type<T>, TList>::value;	}

#endif

////////////////////////////////////////////////////////////////////////////////

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

static constexpr
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

	constexpr TransitionBase(const StateID destination_) noexcept
		: destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TransitionBase(const StateID origin_,
							 const StateID destination_) noexcept
		: origin	 {origin_}
		, destination{destination_}
	{}

	//----------------------------------------------------------------------

	constexpr bool operator == (const TransitionBase& other) const noexcept {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	//----------------------------------------------------------------------

	constexpr void clear() noexcept {
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

	constexpr TransitionT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TransitionT(const StateID destination,
						  const Payload& payload) noexcept
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TransitionT(const StateID destination,
						  Payload&& payload) noexcept
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	constexpr TransitionT(const StateID origin,
						  const StateID destination,
						  const Payload& payload) noexcept
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TransitionT(const StateID origin,
						  const StateID destination,
						  Payload&& payload) noexcept
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	constexpr bool operator == (const TransitionT& other) const noexcept {
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

#pragma pack(push, 2)

struct TaskBase {
	constexpr TaskBase() noexcept {}

	constexpr TaskBase(const StateID origin_,
					   const StateID destination_) noexcept
		: origin{origin_}
		, destination{destination_}
	{}

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
};

constexpr bool operator == (const TaskBase& lhs, const TaskBase& rhs) noexcept {
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

	constexpr TaskT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TaskT(const StateID origin,
					const StateID destination,
					const Payload& payload) noexcept
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TaskT(const StateID origin,
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
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
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

	constexpr void clearTaskStatus(const StateID stateId) noexcept;

#ifdef FFSM2_ENABLE_ASSERT
	constexpr void verifyEmptyStatus(const StateID stateId) const noexcept;

	constexpr void verifyPlans() const noexcept;
	constexpr Long verifyPlan()  const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
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

	constexpr void clearTaskStatus(const StateID stateId) noexcept;

#ifdef FFSM2_ENABLE_ASSERT
	constexpr void verifyEmptyStatus(const StateID stateId) const noexcept;

	constexpr void verifyPlans() const noexcept;
	constexpr Long verifyPlan()  const noexcept;
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
constexpr void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
constexpr void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::verifyEmptyStatus(const StateID stateId) const noexcept {
	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
constexpr void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
constexpr Long
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::verifyPlan() const noexcept {
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

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
constexpr void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
constexpr void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::verifyEmptyStatus(const StateID stateId) const noexcept {
	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
constexpr void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
constexpr Long
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::verifyPlan() const noexcept {
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

	constexpr Status(const Result result_ = Result::NONE) noexcept;

	constexpr explicit operator bool() const noexcept		{ return result != Result::NONE;	}

	constexpr void clear() noexcept;
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

constexpr Status
combine(const Status lhs, const Status rhs) noexcept {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
class ConstPlanT {
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
		constexpr Iterator(const ConstPlanT& plan)		noexcept;

		constexpr explicit operator bool() const		noexcept;

		constexpr void operator ++()					noexcept;

		constexpr const Task& operator  *() const		noexcept	{ return  _plan._planData.tasks[_curr];		}
		constexpr const Task* operator ->() const		noexcept	{ return &_plan._planData.tasks[_curr];		}

		constexpr Long next() const						noexcept;

		const ConstPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	constexpr ConstPlanT(const PlanData& planData)		noexcept;

	template <typename T>
	static constexpr StateID  stateId()					noexcept	{ return			index<StateList , T>();	}

public:
	constexpr explicit operator bool() const			noexcept;

	constexpr Iterator first()							noexcept	{ return Iterator{*this};					}

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
		constexpr Iterator(PlanBaseT& plan)				noexcept;

		constexpr explicit operator bool() const		noexcept;

		constexpr void operator ++()					noexcept;

		constexpr		Task& operator  *()				noexcept	{ return  _plan._planData.tasks[_curr];		}
		constexpr const Task& operator  *() const		noexcept	{ return  _plan._planData.tasks[_curr];		}

		constexpr		Task* operator ->()				noexcept	{ return &_plan._planData.tasks[_curr];		}
		constexpr const Task* operator ->() const		noexcept	{ return &_plan._planData.tasks[_curr];		}

		constexpr void remove()							noexcept;

		constexpr Long next() const						noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator {
		constexpr CIterator(const PlanBaseT& plan)		noexcept;

		constexpr explicit operator bool() const		noexcept;

		constexpr void operator ++()					noexcept;

		constexpr		Task& operator  *()				noexcept	{ return  _plan._planData.tasks[_curr];		}
		constexpr const Task& operator  *() const		noexcept	{ return  _plan._planData.tasks[_curr];		}

		constexpr		Task* operator ->()				noexcept	{ return &_plan._planData.tasks[_curr];		}
		constexpr const Task* operator ->() const		noexcept	{ return &_plan._planData.tasks[_curr];		}

		constexpr Long next() const						noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	constexpr PlanBaseT(PlanData& planData)				noexcept;

	template <typename T>
	static constexpr StateID  stateId()					noexcept	{ return			index<StateList , T>();	}

	constexpr bool append(const StateID origin,
						  const StateID destination)	noexcept;

	constexpr bool linkTask(const Long index)			noexcept;

public:
	constexpr explicit operator bool() const			noexcept;

	/// @brief Clear all tasks from the plan
	constexpr void clear()								noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	constexpr bool change   (const StateID origin, const StateID destination)	noexcept	{ return append(origin, destination);							}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	constexpr bool change   (const StateID destination)							noexcept	{ return change(stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	constexpr bool change   ()													noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	constexpr	   Iterator first()												noexcept	{ return	  Iterator{*this};									}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	constexpr CIterator first() const											noexcept	{ return CIterator{*this};										}

private:
	constexpr void remove(const Long task)										noexcept;

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
		, typename TPayload>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, NSubstitutionLimit
				, NTaskCapacity
				, TPayload>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
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
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	constexpr bool append(const StateID origin,
						  const StateID destination,
						  const Payload& payload) noexcept;

	constexpr bool append(const StateID origin,
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
	constexpr bool changeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append		  (origin								, destination								,  			payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	constexpr bool changeWith   (const StateID origin, const StateID destination,	   Payload&& payload) noexcept	{ return append		  (origin								, destination								, std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	constexpr bool changeWith   (					   const StateID destination, const Payload& payload) noexcept	{ return changeWith   (PlanBase::template stateId<TOrigin>(), destination								, 			payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	constexpr bool changeWith   (					   const StateID destination,	   Payload&& payload) noexcept	{ return changeWith   (PlanBase::template stateId<TOrigin>(), destination								, std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	constexpr bool changeWith   (												  const Payload& payload) noexcept	{ return changeWith   (PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), 			payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	constexpr bool changeWith   (													   Payload&& payload) noexcept	{ return changeWith   (PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, NSubstitutionLimit
				, NTaskCapacity
				, void>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
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

constexpr
Status::Status(const Result result_) noexcept
	: result{result_}
{}

//------------------------------------------------------------------------------

constexpr void
Status::clear() noexcept {
	result = Result::NONE;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
constexpr
ConstPlanT<TArgs>::Iterator::Iterator(const ConstPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr
ConstPlanT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < ConstPlanT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < ConstPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr void
ConstPlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr Long
ConstPlanT<TArgs>::Iterator::next() const noexcept {
	if (_curr < ConstPlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
constexpr
ConstPlanT<TArgs>::ConstPlanT(const PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr
ConstPlanT<TArgs>::operator bool() const noexcept {
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
constexpr
PlanBaseT<TArgs>::Iterator::Iterator(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr
PlanBaseT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr void
PlanBaseT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr void
PlanBaseT<TArgs>::Iterator::remove() noexcept {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr Long
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
constexpr
PlanBaseT<TArgs>::CIterator::CIterator(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr
PlanBaseT<TArgs>::CIterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr void
PlanBaseT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
constexpr Long
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
constexpr
PlanBaseT<TArgs>::PlanBaseT(PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
constexpr
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
constexpr bool
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
constexpr bool
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
constexpr void
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
constexpr void
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

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
constexpr bool
PlanT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::append(const StateID origin,
												 const StateID destination,
												 const Payload& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, payload));
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
constexpr bool
PlanT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::append(const StateID origin,
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
		FFSM2_IF_PLANS(, Long)
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry {
	constexpr void clearRequests() noexcept {
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
	using TransitionSets	= Array<Transition, TArgs::SUBSTITUTION_LIMIT>;

#ifdef FFSM2_ENABLE_PLANS
	using PlanData			= PlanDataT<TArgs>;
	using ConstPlan			= ConstPlanT<TArgs>;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		constexpr Origin(ControlT& control_,
						 const StateID stateId) noexcept;

		~Origin() noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr ControlT(Context& context
					 , Registry& registry
					 , Transition& request
					 FFSM2_IF_PLANS(, PlanData& planData)
					 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: _context{context}
		, _registry{registry}
		, _request{request}
		FFSM2_IF_PLANS(, _planData{planData})
		FFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()			noexcept	{ return index<StateList, TState>();		}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	constexpr		Context& _()				noexcept	{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	constexpr const Context& _() const			noexcept	{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	constexpr		Context& context()			noexcept	{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	constexpr const Context& context() const	noexcept	{ return _context;							}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	constexpr const Transition& request() const	noexcept	{ return _request;							}

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Access read-only plan
	/// @return Plan
	constexpr ConstPlan plan() const			noexcept	{ return ConstPlan{_planData};				}

#endif

	//----------------------------------------------------------------------

protected:
#ifdef FFSM2_ENABLE_LOG_INTERFACE
	constexpr Logger* logger()					noexcept	{ return _logger;							}
#endif

protected:
	Context& _context;
	Registry& _registry;
	Transition& _request;
	StateID _originId = INVALID_STATE_ID;
	FFSM2_IF_PLANS(PlanData& _planData);
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

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;

#ifdef FFSM2_ENABLE_PLANS
	using typename Control::PlanData;
	using typename Control::ConstPlan;

	using Plan			= PlanT<TArgs>;
#endif

	using Control::Control;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Access plan
	/// @return Plan
	constexpr	   Plan plan()					noexcept	{ return	  Plan{_planData};				}

	/// @brief Access read-only plan
	/// @return Read-only plan
	constexpr ConstPlan plan() const			noexcept	{ return ConstPlan{_planData};				}

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
	using TasksBits		= BitArray<StateID, StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		constexpr Lock(FullControlBaseT& control_) noexcept;
		~Lock() noexcept;

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
	constexpr void changeTo (const StateID stateId) noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	constexpr void changeTo () noexcept		{ changeTo (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Succeed a plan task for the current state
	constexpr void succeed() noexcept;

	/// @brief Fail a plan task for the current state
	constexpr void fail()	 noexcept;

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
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
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
	constexpr void updatePlan(TState& headState, const Status subStatus) noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	constexpr void changeWith   (const StateID  stateId,
								 const Payload& payload) noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	constexpr void changeWith   (const StateID  stateId,
									  Payload&& payload) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	constexpr void changeWith   (const Payload& payload) noexcept	{ changeWith   (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	constexpr void changeWith   (	  Payload&& payload) noexcept	{ changeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}

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
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
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
	constexpr void updatePlan(TState& headState, const Status subStatus) noexcept;

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
	using typename FullControl::TransitionSets;

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControl::PlanData;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr GuardControlT(Context& context
						  , Registry& registry
						  , Transition& request
						  , const TransitionSets& currentTransitions
						  , const Transition& pendingTransition
						  FFSM2_IF_PLANS(, PlanData& planData)
						  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, request
					FFSM2_IF_PLANS(, planData)
					FFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransitions{currentTransitions}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr const TransitionSets& currentTransitions() const	noexcept	{ return _currentTransitions;	}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	constexpr Transition& pendingTransition() const				noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	constexpr void cancelPendingTransition()					noexcept;

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
constexpr
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
constexpr
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
constexpr void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
constexpr void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
constexpr void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
template <typename TState>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::updatePlan(TState& headState,
															const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan())
			p.clear();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																			 const Payload& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																				  Payload&& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
template <typename TState>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::updatePlan(TState& headState,
															 const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan())
			p.clear();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
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
constexpr void
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
	constexpr void preEntryGuard(Context&) noexcept {}

	constexpr void preEnter		(Context&) noexcept {}
	constexpr void preReenter	(Context&) noexcept {}

	constexpr void preUpdate	(Context&) noexcept {}

	template <typename TEvent>
	constexpr void preReact		(const TEvent&,
								 Context&) noexcept {}

	constexpr void preExitGuard	(Context&) noexcept {}

	constexpr void postExit		(Context&) noexcept {}

	template <typename T>
	static constexpr StateID  stateId()	   noexcept { return index<StateList, T>();	}
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

	constexpr void widePreEntryGuard(Context& context) noexcept;

	constexpr void widePreEnter		(Context& context) noexcept;
	constexpr void widePreReenter	(Context& context) noexcept;

	constexpr void widePreUpdate	(Context& context) noexcept;

	template <typename TEvent>
	constexpr void widePreReact		(const TEvent& event,
									 Context& context) noexcept;

	constexpr void widePreExitGuard	(Context& context) noexcept;

	constexpr void widePostExit		(Context& context) noexcept;
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

	constexpr void entryGuard		(GuardControl&)	noexcept {}

	constexpr void enter			(PlanControl& )	noexcept {}
	constexpr void reenter			(PlanControl& )	noexcept {}

	constexpr void update			(FullControl& )	noexcept {}

	template <typename TEvent>
	constexpr void react			(const TEvent&,
									 FullControl& )	noexcept {}

	constexpr void exitGuard		(GuardControl&)	noexcept {}

	constexpr void exit				(PlanControl& )	noexcept {}

#ifdef FFSM2_ENABLE_PLANS
	constexpr void planSucceeded	(FullControl& control) noexcept { control.succeed();	}
	constexpr void planFailed		(FullControl& control) noexcept { control.fail();		}
#endif

	constexpr void widePreEntryGuard(Context& context) noexcept;

	constexpr void widePreEnter		(Context& context) noexcept;
	constexpr void widePreReenter	(Context& context) noexcept;

	constexpr void widePreUpdate	(Context& context) noexcept;

	template <typename TEvent>
	constexpr void widePreReact		(const TEvent& event,
									 Context& context) noexcept;

	constexpr void widePreExitGuard	(Context& context) noexcept;

	constexpr void widePostExit		(Context& context) noexcept;
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
constexpr void
B_<TF, TR...>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
constexpr void
B_<TF, TR...>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
constexpr void
B_<TF, TR...>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
constexpr void
B_<TF, TR...>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
constexpr void
B_<TF, TR...>::widePreReact(const TEvent& event,
							Context& context) noexcept
{
	TF::preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
constexpr void
B_<TF, TR...>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
constexpr void
B_<TF, TR...>::widePostExit(Context& context) noexcept {
	TF::postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
constexpr void
B_<TF>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
constexpr void
B_<TF>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
constexpr void
B_<TF>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
constexpr void
B_<TF>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
constexpr void
B_<TF>::widePreReact(const TEvent& event,
					 Context& context) noexcept
{
	TF::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
constexpr void
B_<TF>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
constexpr void
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
	static constexpr void execute(ffsm2::detail::GuardControlT<TArgs>&) noexcept {}
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

	constexpr DynamicBox() noexcept {}
	~DynamicBox()		   noexcept {}

	constexpr void guard(GuardControlT<TArgs>& control) noexcept	{ Guard<Type>::execute(control);	}

	constexpr void construct() noexcept;
	constexpr void destruct()  noexcept;

	constexpr		Type& get()		  noexcept				{ FFSM2_ASSERT(initialized_); return t_;	}
	constexpr const Type& get() const noexcept				{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare() noexcept	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	constexpr void guard(GuardControlT<TArgs>& control) noexcept;

	constexpr void construct() noexcept 																{}
	constexpr void destruct()  noexcept 																{}

	constexpr		Type& get()		  noexcept						{ return t_;						}
	constexpr const Type& get() const noexcept						{ return t_;						}

	FFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = typename std::conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DynamicBox<T, TArgs>,
					 StaticBox <T, TArgs>
				 >::type;
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
constexpr void
DynamicBox<T, TA>::construct() noexcept {
	FFSM2_ASSERT(!initialized_);

	new(&t_) T{};

	FFSM2_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
constexpr void
DynamicBox<T, TA>::destruct() noexcept {
	FFSM2_ASSERT(initialized_);

	t_.~T();

	FFSM2_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
constexpr void
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

	constexpr bool	 deepEntryGuard		  (GuardControl& control) noexcept;

	constexpr void	 deepConstruct		  (PlanControl&  control) noexcept;

	constexpr void	 deepEnter			  (PlanControl&  control) noexcept;
	constexpr void	 deepReenter		  (PlanControl&  control) noexcept;

	constexpr Status deepUpdate			  (FullControl&  control) noexcept;

	template <typename TEvent>
	constexpr Status deepReact			  (FullControl&	 control,
										   const TEvent& event)	  noexcept;

	constexpr bool	 deepExitGuard		  (GuardControl& control) noexcept;

	constexpr void	 deepExit			  (PlanControl&	 control) noexcept;

	constexpr void	 deepDestruct		  (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_PLANS
	constexpr void	 wrapPlanSucceeded	  (FullControl&	 control) noexcept;
	constexpr void	 wrapPlanFailed		  (FullControl&	 control) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void	 deepChangeToRequested(Control&)			  noexcept {}

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
	constexpr void log(TReturn(THost::*)(TParams...),
					   Logger& logger,
					   Context& context,
					   const Method method) const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	constexpr void log(TReturn(Empty::*)(TParams...),
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
constexpr bool
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
constexpr void
S_<N, TA, TH>::deepConstruct(PlanControl& FFSM2_IF_LOG_INTERFACE(control)) noexcept {
	FFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::deepEnter(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
constexpr void
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
constexpr Status
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
constexpr Status
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
constexpr bool
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
constexpr void
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
constexpr void
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
constexpr void
S_<N, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
constexpr void
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
	using StateList			= TypeList<Head>;

	static constexpr Short WIDTH		  = 1;

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
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
	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_PLANS
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload	= TPayload;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <StateID, typename, Short, typename...>
struct CS_;

template <typename, typename>
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

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT	= TConfig::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_PLANS
	static constexpr Long  TASK_CAPACITY		= TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload		= typename TConfig::Payload;

#ifdef FFSM2_ENABLE_PLANS
	using Task			= typename TConfig::Task;
#endif

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , SUBSTITUTION_LIMIT
							  FFSM2_IF_PLANS(, TASK_CAPACITY)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<TConfig, Apex>;

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
struct CSubMaterialT<N, TA, NI, TypeList<TS...>> {
	using Type = CS_<N, TA, NI,			 TS...>;
};

template <StateID N, typename TA, Short NI, typename TList>
using CSubMaterial = typename CSubMaterialT<N, TA, NI, TList>::Type;

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

	using LStateList	= LHalf<TStates...>;
	using LMaterial		= CSubMaterial<INITIAL_ID,
									   Args,
									   L_PRONG,
									   LStateList>;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using RStateList	= RHalf<TStates...>;
	using RMaterial		= CSubMaterial<INITIAL_ID + LStateList::SIZE,
									   Args,
									   R_PRONG,
									   RStateList>;

	//----------------------------------------------------------------------

	constexpr bool	 wideEntryGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideConstruct		  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideEnter			  (PlanControl&  control, const Short prong) noexcept;
	constexpr void	 wideReenter		  (PlanControl&  control, const Short prong) noexcept;

	constexpr Status wideUpdate			  (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	constexpr Status wideReact			  (FullControl&  control,
										   const TEvent& event,	  const Short prong) noexcept;

	constexpr bool	 wideExitGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideExit			  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideDestruct		  (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void	 wideChangeToRequested(PlanControl&  control, const Short prong) noexcept;

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

	constexpr bool	 wideEntryGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideConstruct		  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideEnter			  (PlanControl&  control, const Short prong) noexcept;
	constexpr void	 wideReenter		  (PlanControl&  control, const Short prong) noexcept;

	constexpr Status wideUpdate			  (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	constexpr Status wideReact			  (FullControl&  control,
										   const TEvent& event,	  const Short prong) noexcept;

	constexpr bool	 wideExitGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideExit			  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideDestruct		  (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void	 wideChangeToRequested(PlanControl&  control, const Short prong) noexcept;

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
constexpr bool
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
constexpr void
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
constexpr void
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
constexpr void
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
constexpr Status
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
constexpr Status
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
constexpr bool
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
constexpr void
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
constexpr void
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
constexpr void
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
constexpr bool
CS_<N, TA, NI, T>::wideEntryGuard(GuardControl& control,
								  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
constexpr void
CS_<N, TA, NI, T>::wideConstruct(PlanControl& control,
								 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
constexpr void
CS_<N, TA, NI, T>::wideEnter(PlanControl& control,
							 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, Short NI, typename T>
constexpr void
CS_<N, TA, NI, T>::wideReenter(PlanControl& control,
							   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
constexpr Status
CS_<N, TA, NI, T>::wideUpdate(FullControl& control,
							  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
template <typename TEvent>
constexpr Status
CS_<N, TA, NI, T>::wideReact(FullControl& control,
							 const TEvent& event,
							 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
constexpr bool
CS_<N, TA, NI, T>::wideExitGuard(GuardControl& control,
								 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
constexpr void
CS_<N, TA, NI, T>::wideExit(PlanControl& control,
							const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
constexpr void
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

	constexpr bool deepForwardEntryGuard(GuardControl& control) noexcept;
	constexpr bool deepEntryGuard		(GuardControl& control) noexcept;

	constexpr void deepConstruct		(PlanControl&  control) noexcept;

	constexpr void deepEnter			(PlanControl&  control) noexcept;

	constexpr void deepUpdate			(FullControl&  control) noexcept;

	template <typename TEvent>
	constexpr void deepReact			(FullControl&  control,
										 const TEvent& event)	noexcept;

	constexpr bool deepForwardExitGuard	(GuardControl& control) noexcept;
	constexpr bool deepExitGuard		(GuardControl& control) noexcept;

	constexpr void deepExit				(PlanControl&  control) noexcept;

	constexpr void deepDestruct			(PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void deepChangeToRequested(PlanControl&  control) noexcept;

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
constexpr bool
C_<TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.active != INVALID_SHORT);

	const Short  requested  = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _subStates.wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
constexpr bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
constexpr void
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
constexpr void
C_<TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
constexpr void
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
constexpr void
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
constexpr bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	const Short  active  = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	return _subStates.wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
constexpr bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
constexpr void
C_<TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
constexpr void
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

template <typename TA, typename TH, typename... TS>
constexpr void
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
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct G_ {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_PLANS
	static constexpr Long TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#ifdef FFSM2_ENABLE_PLANS
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, N				   FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT				  , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), T      >;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>> {
	using Cfg = G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	/// @brief Composite region
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Composite		 = CI_<THead, TSubStates...>;

	/// @brief Headless composite region
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using CompositePeers = CI_<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Root
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root			 = RF_<Cfg, Composite  <THead, TSubStates...>>;

	/// @brief Headless root
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot		 = RF_<Cfg, CompositePeers  <  TSubStates...>>;

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
		FFSM2_IF_PLANS(, Long NTaskCapacity = INVALID_LONG)
		, typename TPayload = void>
using ConfigT = detail::G_<FFSM2_FEATURE_TAG, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

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
template <typename TConfig,
		  typename TApex>
class R_ {
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

	using Payload				= typename TConfig::Payload;

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

	constexpr explicit R_(Context& context
						  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	~R_() noexcept;

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId() noexcept						{ return index<StateList, TState>();	}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	constexpr void update() noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	constexpr void react(const TEvent& event) noexcept;

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	constexpr StateID activeStateId() const noexcept				{ return _registry.active;				}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	constexpr void changeTo(const StateID stateId) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	template <typename TState>
	constexpr void changeTo() noexcept								{ changeTo (stateId<TState>());			}

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	constexpr void attachLogger(Logger* const logger) noexcept		{ _logger = logger;						}

#endif

	//----------------------------------------------------------------------

private:
	constexpr void initialEnter() noexcept;
	constexpr void processTransitions(TransitionSets& currentTransitions) noexcept;

	constexpr bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
										  const Transition& pendingTransition) noexcept;

	constexpr bool cancelledByGuards(const TransitionSets& currentTransitions,
									 const Transition& pendingTransition) noexcept;

protected:
	Context& _context;

	Registry _registry;
	FFSM2_IF_PLANS(PlanData _planData);

	Transition _request;

	MaterialApex _apex;

	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Payload				= TPayload;
	using Transition			= TransitionT<Payload>;

public:
	using Base::Base;

	using Base::stateId;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId() noexcept							{ return Base::template stateId<TState>();		}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	constexpr void changeWith   (const StateID  stateId,
								 const Payload& payload) noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	constexpr void changeWith   (const StateID  stateId,
									  Payload&& payload) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	constexpr void changeWith   (const Payload& payload) noexcept	{ changeWith   (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	constexpr void changeWith   (	  Payload&& payload) noexcept	{ changeWith   (stateId<TState>(), std::move(payload));	}

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
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class RW_ final
	: public	 RP_<TConfig, TApex>
{
	using Base = RP_<TConfig, TApex>;

public:
	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == ::ffsm2::EmptyContext

template <
		  Long NSubstitutionLimit,
		  FFSM2_IF_PLANS(Long NTaskCapacity,)
		  typename TPayload,
		  typename TApex>
class RW_		<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public RP_<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, ::ffsm2::EmptyContext
{
	using Cfg =  ::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	using Context	= typename Cfg::Context;
	using Base		= RP_<Cfg, TApex>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:
	explicit constexpr RW_(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
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

template <typename TG, typename TA>
constexpr
R_<TG, TA>::R_(Context& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() noexcept {
	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
constexpr void
R_<TG, TA>::update() noexcept {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
constexpr void
R_<TG, TA>::react(const TEvent& event) noexcept {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
constexpr void
R_<TG, TA>::changeTo(const StateID stateId) noexcept {
	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
constexpr void
R_<TG, TA>::initialEnter() noexcept {
	FFSM2_ASSERT(_request.destination == INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
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

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
constexpr void
R_<TG, TA>::processTransitions(TransitionSets& currentTransitions) noexcept {
	FFSM2_ASSERT(_request.destination != INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
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

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
constexpr bool
R_<TG, TA>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
								   const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_PLANS(, _planData)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
constexpr bool
R_<TG, TA>::cancelledByGuards(const TransitionSets& currentTransitions,
							  const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_PLANS(, _planData)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
constexpr void
RP_<G_<NFT, TC, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																const Payload& payload) noexcept
{
	_request = Transition{stateId, payload};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
constexpr void
RP_<G_<NFT, TC, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
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
