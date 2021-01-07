// FFSM2 (flat state machine for games and interactive applications)
// 0.4.0 (2021-01-07)
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
#define FFSM2_VERSION_MINOR 4
#define FFSM2_VERSION_PATCH 0

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef FFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif
#include <type_traits>		// move(), forward()

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

#include "detail/shared/macros_on.hpp"

#include "detail/shared/utility.hpp"
#include "detail/shared/iterator.hpp"
#include "detail/shared/array.hpp"
#include "detail/shared/bit_array.hpp"
#include "detail/shared/list.hpp"
#include "detail/shared/type_list.hpp"
#include "detail/shared/type_table.hpp"
#include "detail/shared/value_list.hpp"
#include "detail/shared/value_table.hpp"

#include "detail/features/common.hpp"
#include "detail/features/logger_interface.hpp"

#include "detail/root/plan_data.hpp"
#include "detail/root/plan.hpp"
#include "detail/root/registry.hpp"
#include "detail/root/control.hpp"

#include "detail/structure/injections.hpp"
#include "detail/structure/state_box.hpp"
#include "detail/structure/state.hpp"
#include "detail/structure/forward.hpp"
#include "detail/structure/composite_sub.hpp"
#include "detail/structure/composite.hpp"
#include "detail/root/state_access.hpp"

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag, typename, Long FFSM2_IF_DYNAMIC_PLANS(, Long), typename>
struct GW_;

//------------------------------------------------------------------------------

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
	using ContextT			 = GW_<FEATURE_TAG, T	   , SUBSTITUTION_LIMIT FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = GW_<FEATURE_TAG, Context, N					FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY), Payload>;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = GW_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT						   , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = GW_<FEATURE_TAG, Context, SUBSTITUTION_LIMIT FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY), T      >;
};

//------------------------------------------------------------------------------

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct GW_
	: G_  <NFeatureTag, TContext , NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>
{};

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct GW_<NFeatureTag, TContext&, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>
	: G_  <NFeatureTag, TContext&, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>
{};

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct GW_<NFeatureTag, TContext*, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>
	: G_  <NFeatureTag, TContext*, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>
{};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <GW_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>> {
	using Cfg = GW_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>;

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
using ConfigT = detail::GW_<FFSM2_FEATURE_TAG, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>;

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

#include "detail/root.hpp"

#include "detail/shared/macros_off.hpp"
