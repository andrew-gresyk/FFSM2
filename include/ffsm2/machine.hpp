// HFSM2 (hierarchical state machine for games and interactive applications)
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

	#define FFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)				\
		if (_logger)															\
			_logger->recordTaskStatus(CONTEXT, REGION, ORIGIN, STATUS)

	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)						\
		if (_logger)															\
			_logger->recordPlanStatus(CONTEXT, REGION, STATUS)

#endif

	#define FFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)						\
		if (_logger)															\
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#else

	#define FFSM2_IF_LOG_INTERFACE(...)
	#define FFSM2_LOG_INTERFACE_MASK									 (0 << 5)

	#define FFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, DESTINATION)

#ifdef FFSM2_ENABLE_PLANS
	#define FFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
	#define FFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
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
void
fill(T& a, const char value) {
	memset(&a, (int) value, sizeof(a));
}

//------------------------------------------------------------------------------

template <typename T, unsigned NCount>
constexpr
unsigned
count(const T(&)[NCount]) {
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
constexpr
T
min(const T t1, const T t2) { return t1 < t2 ? t1 : t2; }


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

constexpr
Long
roundUp(const Long x,
		const Long to)
{
	return (x + (to - 1)) / to;
}

//------------------------------------------------------------------------------

constexpr
Short
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
void
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
TO convert(const TI& in) {
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
						  const Long cursor)
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const Iterator<Container>& dummy) const;

	FFSM2_INLINE Iterator& operator ++();

	FFSM2_INLINE	   Item& operator *()		{ return  _container[_cursor]; }
	FFSM2_INLINE const Item& operator *() const { return  _container[_cursor]; }

	FFSM2_INLINE	   Item* operator->()		{ return &_container[_cursor]; }
	FFSM2_INLINE const Item* operator->() const { return &_container[_cursor]; }

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
						  const Long cursor)
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const Iterator<const Container>& dummy) const;

	FFSM2_INLINE Iterator& operator ++();

	FFSM2_INLINE const Item& operator *() const { return _container[_cursor]; }

	FFSM2_INLINE const Item* operator->() const { return &operator *();		 }

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
Iterator<T>::operator != (const Iterator<T>& FFSM2_IF_ASSERT(dummy)) const {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
Iterator<T>&
Iterator<T>::operator ++() {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
Iterator<const T>::operator != (const Iterator<const T>& FFSM2_IF_ASSERT(dummy)) const {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
Iterator<const T>&
Iterator<const T>::operator ++() {
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
class Array {
	template <typename>
	friend class Iterator;

public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	FFSM2_INLINE void clear()														{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	FFSM2_INLINE Long append(const TValue& value);

	template <typename TValue>
	FFSM2_INLINE Long append(TValue&& value);

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i);

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i) const;

	FFSM2_INLINE Long count() const													{ return _count;	}

	Array& operator += (const Item& item);
	Array& operator += (Item&& item);

	template <Long N>
	Array& operator += (const Array<Item, N>& other);

	FFSM2_INLINE Iterator<      Array>  begin()			{ return Iterator<		Array>(*this,     0);	}
	FFSM2_INLINE Iterator<const Array>  begin() const	{ return Iterator<const Array>(*this,     0);	}
	FFSM2_INLINE Iterator<const Array> cbegin() const	{ return Iterator<const Array>(*this,     0);	}

	FFSM2_INLINE Iterator<      Array>	  end()			{ return Iterator<		Array>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const Array>	  end() const	{ return Iterator<const Array>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const Array>   cend() const	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	FFSM2_INLINE Long next(const Long i) const										{ return i + 1;		}
	FFSM2_INLINE Long limit() const													{ return _count;	}

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
template <typename TValue>
Long
Array<T, NC>::append(const TValue& value) {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
Long
Array<T, NC>::append(TValue&& value) {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
Array<T, NC>::operator[] (const N i) {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
Array<T, NC>::operator[] (const N i) const {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
Array<T, NC>&
Array<T, NC>::operator += (const Item& item) {
	append(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
Array<T, NC>&
Array<T, NC>::operator += (Item&& item) {
	append(std::move(item));

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
Array<T, NC>&
Array<T, NC>::operator += (const Array<T, N>& other) {
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct ITL_
	: private ITL_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = ITL_Impl<IndexSequenceFor<Ts...>, Ts...>;

	static constexpr Long SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<ITL_EntryT<T>, ITL_>::value; }

	template <typename T>
	static constexpr Long index() {
		return Base::template select<T>(ITL_{});
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, ITL_<Ts...>> {
	using Type = ITL_<T, Ts...>;
};

template <typename... Ts>
using Prepend = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<ITL_<Ts1...>, ITL_<Ts2...>> {
	using Type = ITL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct LesserT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct LesserT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LesserT<H, I + 1, TRest...>::Type>,
										   typename LesserT<H, I + 1, TRest...>::Type>::type;
};

template <Long H, Long I>
struct LesserT<H, I> {
	using Type = ITL_<>;
};

template <typename... Ts>
using SplitL = typename LesserT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct GreaterT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct GreaterT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename GreaterT<H, I + 1, TRest...>::Type,
										   ITL_<TFirst, TRest...>>::type;
};

template <Long H, Long I>
struct GreaterT<H, I> {
	using Type = ITL_<>;
};

template <typename... Ts>
using SplitR = typename GreaterT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

}
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

	COUNT
};

//------------------------------------------------------------------------------

static inline
const char*
stateName(const std::type_index stateType) {
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
methodName(const Method method) {
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
	FFSM2_INLINE TransitionBase() = default;

	//----------------------------------------------------------------------

	FFSM2_INLINE TransitionBase(const StateID destination_)
		: destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionBase(const StateID origin_,
								const StateID destination_)
		: origin{origin_}
		, destination{destination_}
	{}

	//----------------------------------------------------------------------

	FFSM2_INLINE bool operator == (const TransitionBase& other) const {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	//----------------------------------------------------------------------

	FFSM2_INLINE void clear() {
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

	FFSM2_INLINE TransitionT() = default;

	using TransitionBase::TransitionBase;

	//----------------------------------------------------------------------

	FFSM2_INLINE TransitionT(const StateID destination,
							 const Payload& payload)
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionT(const StateID destination,
							 Payload&& payload)
		: TransitionBase{destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	FFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 const Payload& payload)
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 Payload&& payload)
		: TransitionBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	FFSM2_INLINE bool operator == (const TransitionT& other) const {
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

	virtual void recordMethod(Context& /*context*/,
							  const StateID /*origin*/,
							  const Method /*method*/)
	{}

	virtual void recordTransition(Context& /*context*/,
								  const StateID /*origin*/,
								  const StateID /*target*/)
	{}

	virtual void recordCancelledPending(Context& /*context*/,
										const StateID /*origin*/)
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

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

}
}

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

	inline Status(const Result result_ = Result::NONE);

	inline explicit operator bool() const	{ return result != Result::NONE; }

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const Result result_)
	: result{result_}
{}

//------------------------------------------------------------------------------

void
Status::clear() {
	result = Result::NONE;
}

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
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry
{
	void clearRequests() {
		requested = INVALID_SHORT;
	}

	Short requested	= INVALID_SHORT;
	Short active	= INVALID_SHORT;
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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		FFSM2_INLINE Origin(ControlT& control_,
							const StateID stateId);

		FFSM2_INLINE ~Origin();

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE ControlT(Context& context
						, Registry& registry
						, Transition& request
						FFSM2_IF_LOG_INTERFACE(, Logger* const logger))
		: _context{context}
		, _registry{registry}
		, _request{request}
		FFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()				{ return			StateList ::template index<TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_INLINE	   Context& _()									{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_INLINE const Context& _() const							{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_INLINE	   Context& context()							{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_INLINE const Context& context() const						{ return _context;							}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	FFSM2_INLINE const Transition& request() const					{ return _request;							}

	//----------------------------------------------------------------------

protected:
#ifdef FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_INLINE Logger* logger()									{ return _logger;							}
#endif

protected:
	Context& _context;
	Registry& _registry;
	Transition& _request;
	StateID _originId = INVALID_STATE_ID;
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

	using Control::Control;

protected:
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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		FFSM2_INLINE Lock(FullControlBaseT& control_);
		FFSM2_INLINE ~Lock();

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// Clang trips over 'stateId<>()', so give it a hint it comes from PlanControl

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	FFSM2_INLINE void changeTo (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	FFSM2_INLINE void changeTo ()							{ changeTo (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

protected:
	using PlanControl::_request;
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
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , NSubstitutionLimit
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
					 , TPayload>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

	using FullControlBase::FullControlBase;

public:
	using FullControlBase::context;

	using FullControlBase::changeTo;

	//------------------------------------------------------------------------------

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith   (const Payload& payload)	{ changeWith   (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith   (	 Payload&& payload)	{ changeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}


	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_request;
	FFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , NSubstitutionLimit
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
					 , void>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

	using FullControlBase::FullControlBase;

public:
	using FullControlBase::changeTo;

protected:

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

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE GuardControlT(Context& context
							 , Registry& registry
							 , Transition& request
							 , const TransitionSets& currentTransitions
							 , const Transition& pendingTransition
							 FFSM2_IF_LOG_INTERFACE(, Logger* const logger))
		: FullControl{context
					, registry
					, request
					FFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransitions{currentTransitions}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId State identifier
	/// @return State pending activation/deactivation status
	FFSM2_INLINE bool isPendingChange(const StateID stateId) const	{ return _registry.isPendingChange(stateId);	}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	FFSM2_INLINE bool isPendingChange()			{ return isPendingChange(FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE const TransitionSets& currentTransitions() const	{ return _currentTransitions;					}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	FFSM2_INLINE Transition& pendingTransition() const				{ return _pendingTransition;					}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	FFSM2_INLINE void cancelPendingTransition();

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
								const StateID stateId)
	: control{control_}
	, prevId{control._originId}
{
	control._originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Origin::~Origin() {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_)
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, Long NSL, typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL, TTP>>::changeWith(const StateID stateId,
													   const Payload& payload)
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL, TTP>>::changeWith(const StateID stateId,
													   Payload&& payload)
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
GuardControlT<TArgs>::cancelPendingTransition() {
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

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	FFSM2_INLINE void preEntryGuard(Context&)		{}

	FFSM2_INLINE void preEnter	   (Context&)		{}
	FFSM2_INLINE void preReenter   (Context&)		{}

	FFSM2_INLINE void preUpdate	   (Context&)		{}

	template <typename TEvent>
	FFSM2_INLINE void preReact	   (const TEvent&,
									Context&)		{}

	FFSM2_INLINE void preExitGuard (Context&)		{}

	FFSM2_INLINE void postExit	   (Context&)		{}

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}
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
	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void widePreEntryGuard(Context& context);

	FFSM2_INLINE void widePreEnter	   (Context& context);
	FFSM2_INLINE void widePreReenter   (Context& context);

	FFSM2_INLINE void widePreUpdate	   (Context& context);

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context);

	FFSM2_INLINE void widePreExitGuard (Context& context);

	FFSM2_INLINE void widePostExit	   (Context& context);
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
	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void	 entryGuard		  (GuardControl&)			{}

	FFSM2_INLINE void	 enter			  (PlanControl&)			{}
	FFSM2_INLINE void	 reenter		  (PlanControl&)			{}

	FFSM2_INLINE void	 update			  (FullControl&)			{}

	template <typename TEvent>
	FFSM2_INLINE void	 react			  (const TEvent&,
						 				   FullControl&)			{}

	FFSM2_INLINE void	 exitGuard		  (GuardControl&)			{}

	FFSM2_INLINE void	 exit			  (PlanControl&)			{}

	FFSM2_INLINE void	 widePreEntryGuard(Context& context);

	FFSM2_INLINE void	 widePreEnter	  (Context& context);
	FFSM2_INLINE void	 widePreReenter   (Context& context);

	FFSM2_INLINE void	 widePreUpdate	  (Context& context);

	template <typename TEvent>
	FFSM2_INLINE void	 widePreReact	  (const TEvent& event,
					 	 				   Context& context);

	FFSM2_INLINE void	 widePreExitGuard (Context& context);

	FFSM2_INLINE void	 widePostExit	  (Context& context);
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
B_<TF, TR...>::widePreEntryGuard(Context& context) {
	TF::preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEnter(Context& context) {
	TF::preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreReenter(Context& context) {
	TF::preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreUpdate(Context& context) {
	TF::preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
void
B_<TF, TR...>::widePreReact(const TEvent& event,
							Context& context)
{
	TF::preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreExitGuard(Context& context) {
	TF::preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePostExit(Context& context) {
	TF::postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
void
B_<TF>::widePreEntryGuard(Context& context) {
	TF::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreEnter(Context& context) {
	TF::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
void
B_<TF>::widePreReenter(Context& context) {
	TF::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreUpdate(Context& context) {
	TF::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
void
B_<TF>::widePreReact(const TEvent& event,
					 Context& context)
{
	TF::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreExitGuard(Context& context) {
	TF::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePostExit(Context& context) {
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
	static void execute(ffsm2::detail::GuardControlT<TArgs>&) {}
};

namespace detail {

//------------------------------------------------------------------------------

FFSM2_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DBox final {
	static constexpr bool isBare()								{ return false;					}

	union {
		T t_;
	};

	FFSM2_INLINE  DBox() {}
	FFSM2_INLINE ~DBox() {}

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control)		{ Guard<T>::execute(control);	}

	FFSM2_INLINE void construct();
	FFSM2_INLINE void destruct();

	FFSM2_INLINE	   T& get()						{ FFSM2_ASSERT(initialized_); return t_;	}
	FFSM2_INLINE const T& get() const				{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct SBox final {
	static constexpr bool isBare()	{ return std::is_base_of<T, StaticEmptyT<TArgs>>::value;	}

	T t_;

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control);

	FFSM2_INLINE void construct()																{}
	FFSM2_INLINE void destruct()																{}

	FFSM2_INLINE	   T& get()									{ return t_;					}
	FFSM2_INLINE const T& get() const							{ return t_;					}

	FFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = typename std::conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DBox<T, TArgs>,
					 SBox<T, TArgs>
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
void
DBox<T, TA>::construct() {
	FFSM2_ASSERT(!initialized_);

	new(&t_) T{};

	FFSM2_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
void
DBox<T, TA>::destruct() {
	FFSM2_ASSERT(initialized_);

	t_.~T();

	FFSM2_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
SBox<T, TA>::guard(GuardControlT<TA>& control) {
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

	FFSM2_INLINE bool	 deepEntryGuard		  (GuardControl& control);

	FFSM2_INLINE void	 deepConstruct		  (PlanControl&  control);

	FFSM2_INLINE void	 deepEnter			  (PlanControl&  control);
	FFSM2_INLINE void	 deepReenter		  (PlanControl&  control);

	FFSM2_INLINE Status	 deepUpdate			  (FullControl&  control);

	template <typename TEvent>
	FFSM2_INLINE Status	 deepReact			  (FullControl&	 control, const TEvent& event);

	FFSM2_INLINE bool	 deepExitGuard		  (GuardControl& control);

	FFSM2_INLINE void	 deepExit			  (PlanControl&	 control);

	FFSM2_INLINE void	 deepDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	deepChangeToRequested(Control&)																		{}

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
			 const Method method) const
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	void log(TReturn(Empty::*)(TParams...),
			 Logger&,
			 Context&,
			 const Method) const
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
S_<N, TA, TH>::deepEntryGuard(GuardControl& control) {
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
S_<N, TA, TH>::deepConstruct(PlanControl& FFSM2_IF_LOG_INTERFACE(control)) {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepEnter(PlanControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
void
S_<N, TA, TH>::deepReenter(PlanControl& control) {
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
S_<N, TA, TH>::deepUpdate(FullControl& control) {
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
						 const TEvent& event)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
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
S_<N, TA, TH>::deepExitGuard(GuardControl& control) {
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
S_<N, TA, TH>::deepExit(PlanControl& control) {
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
S_<N, TA, TH>::deepDestruct(PlanControl& FFSM2_IF_LOG_INTERFACE(control)) {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::DESTRUCT);

	_headBox.destruct();
}

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
struct WrapInfoT<	  TH> {
	using Type =  SI_<TH>;
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
	using StateList			= ITL_<Head>;

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
		, typename TPayload>
struct ArgsT final {
	using Context		= TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

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

	using Payload		= typename TConfig::Payload;

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , SUBSTITUTION_LIMIT
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<TConfig, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	//using Injection		= InjectionT<Args>;

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
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG;
	}

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, Short, typename>
struct CSubMaterialT;

template <StateID N, typename TA, Short NI, typename... TS>
struct CSubMaterialT<N, TA, NI, ITL_<TS...>> {
	using Type = CS_<N, TA, NI,	     TS...>;
};

template <StateID N, typename TA, Short NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<N, TA, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename>
struct InfoT;

template <StateID N, typename TA, typename TH>
struct InfoT<S_<N, TA, TH>> {
	using Type = SI_<  TH>;
};

template <typename TA, typename TH, typename... TS>
struct InfoT<C_< TA, TH, TS...>> {
	using Type = CI_<TH, TS...>;
};

template <StateID N, typename TA, Short NI	 , typename... TS>
struct InfoT<CS_<N, TA, NI, TS...>> {
	using Type = CSI_<		TS...>;
};

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

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<INITIAL_ID,
									   Args,
									   L_PRONG,
									   LStates>;
	using LHalfInfo		= typename InfoT<LHalf>::Type;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<INITIAL_ID  + LHalfInfo::STATE_COUNT,
									   Args,
									   R_PRONG,
									   RStates>;

	//----------------------------------------------------------------------

	FFSM2_INLINE bool	 wideEntryGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideConstruct				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideEnter					 (PlanControl&  control,						 const Short prong);
	FFSM2_INLINE void	 wideReenter				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE Status	 wideUpdate					 (FullControl&  control,						 const Short prong);

	template <typename TEvent>
	FFSM2_INLINE Status	 wideReact					 (FullControl&  control, const TEvent& event,	 const Short prong);

	FFSM2_INLINE bool	 wideExitGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideExit					 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideDestruct				 (PlanControl&  control,						 const Short prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	 wideChangeToRequested		 (PlanControl&  control,						 const Short prong);

	//----------------------------------------------------------------------

	LHalf lHalf;
	RHalf rHalf;
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

	FFSM2_INLINE bool	 wideEntryGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideConstruct				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideEnter					 (PlanControl&  control,						 const Short prong);
	FFSM2_INLINE void	 wideReenter				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE Status	 wideUpdate					 (FullControl&  control,						 const Short prong);

	template <typename TEvent>
	FFSM2_INLINE Status	 wideReact					 (FullControl&  control, const TEvent& event,	 const Short prong);

	FFSM2_INLINE bool	 wideExitGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideExit					 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideDestruct				 (PlanControl&  control,						 const Short prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	 wideChangeToRequested		 (PlanControl&  control,						 const Short prong);

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
									  const Short prong)
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
									 const Short prong)
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
								 const Short prong)
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
								   const Short prong)
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
								  const Short prong)
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
								 const Short prong)
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
									 const Short prong)
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
								const Short prong)
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
									const Short prong)
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
											 const Short prong)
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
								  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideConstruct(PlanControl& control,
								 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideEnter(PlanControl& control,
							 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideReenter(PlanControl& control,
							   const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
Status
CS_<N, TA, NI, T>::wideUpdate(FullControl& control,
							  const Short FFSM2_IF_ASSERT(prong))
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
							 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
bool
CS_<N, TA, NI, T>::wideExitGuard(GuardControl& control,
								 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideExit(PlanControl& control,
							const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideDestruct(PlanControl& control,
								const Short FFSM2_IF_ASSERT(prong))
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

	FFSM2_INLINE bool	 deepForwardEntryGuard		   (GuardControl& control);
	FFSM2_INLINE bool	 deepEntryGuard				   (GuardControl& control);

	FFSM2_INLINE void	 deepConstruct				   (PlanControl&  control);

	FFSM2_INLINE void	 deepEnter					   (PlanControl&  control);

	FFSM2_INLINE Status	 deepUpdate					   (FullControl&  control);

	template <typename TEvent>
	FFSM2_INLINE Status	 deepReact					   (FullControl&  control, const TEvent& event);

	FFSM2_INLINE bool	 deepForwardExitGuard		   (GuardControl& control);
	FFSM2_INLINE bool	 deepExitGuard				   (GuardControl& control);

	FFSM2_INLINE void	 deepExit					   (PlanControl&  control);

	FFSM2_INLINE void	 deepDestruct				   (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	 deepChangeToRequested		   (PlanControl&  control);

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
C_<TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) {
	FFSM2_ASSERT(control._registry.active != INVALID_SHORT);

	const Short  requested  = control._registry.requested;
	FFSM2_ASSERT(requested != control._registry.active);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _subStates.wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) {
	const Short requested = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepConstruct(PlanControl& control) {
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
C_<TA, TH, TS...>::deepEnter(PlanControl& control) {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
Status
C_<TA, TH, TS...>::deepUpdate(FullControl& control) {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates		 .wideUpdate(control, active);

		return headStatus;
	} else
		return _subStates.wideUpdate(control, active);

}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
Status
C_<TA, TH, TS...>::deepReact(FullControl& control,
							 const TEvent& event)
{
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (const Status headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};
		_subStates		 .wideReact(control, event, active);

		return headStatus;
	} else
		return _subStates.wideReact(control, event, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) {
	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	const Short  active  = control._registry.active;
	FFSM2_ASSERT(active != control._registry.requested);
	FFSM2_ASSERT(active != INVALID_SHORT);

	return _subStates.wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepExit(PlanControl& control) {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepDestruct(PlanControl& control) {
	Short& active = control._registry.active;

	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideDestruct(control, active);
	_headState.deepDestruct(control);

	active = INVALID_SHORT;
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
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
		, typename TPayload>
struct G_ {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , SUBSTITUTION_LIMIT, Payload>;

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, N                 , Payload>;

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT, T      >;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>> {
	using Cfg = G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	/// @brief Composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Composite		 = CI_<THead, TSubStates...>;

	/// @brief Headless composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using CompositePeers = CI_<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root			 = RF_<Cfg, Composite  <THead, TSubStates...>>;

	/// @brief Headless root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot		 = RF_<Cfg, CompositePeers  <  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

/// @brief Type configuration for MachineT<>
/// @tparam TContext Context type for data shared between states and/or data interface between FSM and external code
/// @tparam TRank Rank type for 'TRank State::rank() const' method
/// @tparam TUtility Utility type for 'TUtility State::utility() const' method
/// @tparam TRNG RNG type used in 'Random' regions
/// @tparam NSubstitutionLimit Maximum number times 'guard()' methods can substitute their states for others
/// @tparam NTaskCapacity Maximum number of tasks across all plans
template <typename TContext = EmptyContext
		, Long NSubstitutionLimit = 4
		, typename TPayload = void>
using ConfigT = detail::G_<FFSM2_FEATURE_TAG, TContext, NSubstitutionLimit, TPayload>;

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

	using MaterialApex			= Material<0, Args, Apex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

public:
	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the hierarchy. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSets		= typename Control::TransitionSets;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	explicit R_(Context& context
				FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr));

	~R_();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()					{ return			StateList ::template index<TState>();	}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update();

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_INLINE void react(const TEvent& event);

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_INLINE StateID activeStateId() const						{ return _registry.active;						}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	FFSM2_INLINE void changeTo(const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_INLINE void changeTo()									{ changeTo (stateId<TState>());					}

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	void attachLogger(Logger* const logger)							{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter();
	void processTransitions(TransitionSets& currentTransitions);

	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
								const Transition& pendingTransition);

	bool cancelledByGuards(const TransitionSets& currentTransitions,
						   const Transition& pendingTransition);

protected:
	Context& _context;

	Registry _registry;
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
		, typename TPayload
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>, TApex>;

	using Payload				= TPayload;
	using Transition			= TransitionT<Payload>;

public:
	using Base::Base;

	using Base::stateId;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()								{ return Base::template stateId<TState>();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith   (const Payload& payload)	{ changeWith   (stateId<TState>(		  payload ));	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith   (	 Payload&& payload)	{ changeWith   (stateId<TState>(std::move(payload)));	}

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
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit, void>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit, void>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit, void>, TApex>;

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
		  typename TPayload,
		  typename TApex>
class RW_		<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit, TPayload>, TApex> final
	: public RP_<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit, TPayload>, TApex>
	, ::ffsm2::EmptyContext
{
	using Cfg =  ::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit, TPayload>;

	using Context	= typename Cfg::Context;
	using Base		= RP_<Cfg, TApex>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:
	explicit FFSM2_INLINE RW_(FFSM2_IF_LOG_INTERFACE(  Logger* const logger = nullptr))
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
R_<TG, TA>::R_(Context& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger))
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() {
	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::update() {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
void
R_<TG, TA>::react(const TEvent& event) {
	FullControl control{_context
					  , _registry
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::changeTo(const StateID stateId) {
	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::initialEnter() {
	FFSM2_ASSERT(_request.destination == INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
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
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::processTransitions(TransitionSets& currentTransitions) {
	FFSM2_ASSERT(_request.destination != INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
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

	if (currentTransitions.count()) {
		_apex.deepChangeToRequested(control);

		_registry.clearRequests();
	}
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
								   const Transition& pendingTransition)
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByGuards(const TransitionSets& currentTransitions,
							  const Transition& pendingTransition)
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, Long NSL, typename TP, typename TA>
void
RP_<G_<NFT, TC, NSL, TP>, TA>::changeWith(const StateID  stateId,
										  const Payload& payload)
{
	_request = Transition{stateId, payload};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

template <FeatureTag NFT, typename TC, Long NSL, typename TP, typename TA>
void
RP_<G_<NFT, TC, NSL, TP>, TA>::changeWith(const StateID  stateId,
										  Payload&& payload)
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
