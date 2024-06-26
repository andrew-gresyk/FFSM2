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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

////////////////////////////////////////////////////////////////////////////////

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
