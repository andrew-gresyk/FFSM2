// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#pragma once

#define FFSM2_ENABLE_LOG_INTERFACE
#define FFSM2_ENABLE_ASSERT
#include <ffsm2/machine.hpp>

#include <doctest/doctest.h>

#include <algorithm> // std::max()
#include <vector>

////////////////////////////////////////////////////////////////////////////////

struct Event final {
	enum class Type {
		ENTRY_GUARD,
		ENTER,
		REENTER,
		PRE_UPDATE,
		UPDATE,
		POST_UPDATE,
		PRE_REACT,
		REACT,
		POST_REACT,
		EXIT_GUARD,
		EXIT,

	//#if FFSM2_PLANS_AVAILABLE()
		PLAN_SUCCEEDED,
		PLAN_FAILED,

		TASK_SUCCESS,
		TASK_FAILURE,
		PLAN_SUCCESS,
		PLAN_FAILURE,
	//#endif

		CHANGE,
		RESTART,
		RESUME,

		SCHEDULE,
		CANCEL_PENDING,

		COUNT
	};

	Event(const ffsm2::StateID origin_,
		  const Type type_,
		  const ffsm2::StateID target_ = ffsm2::INVALID_STATE_ID) noexcept
		: origin{origin_}
		, type{type_}
		, target{target_}
	{}

	Event(const Type type_,
		  const ffsm2::StateID target_) noexcept
		: origin{ffsm2::INVALID_STATE_ID}
		, type{type_}
		, target{target_}
	{}

	ffsm2::StateID origin;
	Type type;
	ffsm2::StateID target;
};
using Events = std::vector<Event>;

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct LoggerT final
	:								 TConfig::LoggerInterface
{
	using Interface		  = typename TConfig::LoggerInterface;

	using typename Interface::Context;

	using typename Interface::Method;
	using typename Interface::StateID;

#if FFSM2_PLANS_AVAILABLE()
	using StatusEvent	  = typename Interface::StatusEvent;
#endif

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) noexcept override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const StateID target) noexcept override;

#if FFSM2_PLANS_AVAILABLE()

	void recordTaskStatus(Context& context,
						  const StateID origin,
						  const StatusEvent event) noexcept override;

	void recordPlanStatus(Context& context,
						  const StatusEvent event) noexcept override;

#endif

	void recordCancelledPending(Context& context,
								const StateID origin) noexcept override;

	void assertSequence(const Events& reference) noexcept;

	Events history;
};
using Logger = LoggerT<::ffsm2::Config>;

//------------------------------------------------------------------------------

using Types = std::vector<ffsm2::StateID>;

////////////////////////////////////////////////////////////////////////////////

#include "tools.inl"
