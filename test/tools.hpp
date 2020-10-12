#pragma once

#define FFSM2_ENABLE_LOG_INTERFACE
#define FFSM2_ENABLE_ASSERT
#include <ffsm2/machine_dev.hpp>

#include <catch2/catch.hpp>

//#include <vector> // already included in catch.hpp

////////////////////////////////////////////////////////////////////////////////

struct Event {
	enum class Type {
		ENTRY_GUARD,
		CONSTRUCT,
		ENTER,
		REENTER,
		UPDATE,
		REACT,
		EXIT_GUARD,
		EXIT,
		DESTRUCT,

	//#ifdef FFSM2_ENABLE_PLANS
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
		  const ffsm2::StateID target_ = ffsm2::INVALID_STATE_ID)
		: origin{origin_}
		, type{type_}
		, target{target_}
	{}

	Event(const Type type_,
		  const ffsm2::StateID target_)
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
struct LoggerT
	:								 TConfig::LoggerInterface
{
	using Interface		  = typename TConfig::LoggerInterface;

	using typename Interface::Context;

	using typename Interface::Method;
	using typename Interface::StateID;

#ifdef FFSM2_ENABLE_PLANS
	using StatusEvent	  = typename Interface::StatusEvent;
#endif

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const StateID target) override;

#ifdef FFSM2_ENABLE_PLANS

	void recordTaskStatus(Context& context,
						  const StateID origin,
						  const StatusEvent event) override;

	void recordPlanStatus(Context& context,
						  const StatusEvent event) override;

#endif

	void recordCancelledPending(Context& context,
								const StateID origin) override;

	void assertSequence(const Events& reference);

	Events history;
};
using Logger = LoggerT<::ffsm2::Config>;

////////////////////////////////////////////////////////////////////////////////

#include "tools.inl"
