#pragma once

#define FFSM2_ENABLE_LOG_INTERFACE
#define FFSM2_ENABLE_ASSERT
#include <ffsm2/machine.hpp>

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

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const StateID target) override;

	void recordCancelledPending(Context& context,
								const StateID origin) override;

	void assertSequence(const Events& reference);

	Events history;
};
using Logger = LoggerT<::ffsm2::Config>;

////////////////////////////////////////////////////////////////////////////////

#include "tools.inl"
