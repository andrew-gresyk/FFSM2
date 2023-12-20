// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_PLANS
#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_plans_verbose {

////////////////////////////////////////////////////////////////////////////////

struct Context {};

using Config = ffsm2::Config
					::ContextT<Context>;

using M = ffsm2::MachineT<Config>;

struct Interruption {};

using Logger = LoggerT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(A),
				S(B),
				S(C),
				S(D)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");
static_assert(FSM::stateId<C>() == 2, "");
static_assert(FSM::stateId<D>() == 3, "");

////////////////////////////////////////////////////////////////////////////////

class Tracked
	: public FSM::State
{
public:
	void entryGuard(GuardControl&) {
		++_entryAttemptCount;
		_currentUpdateCount = 0;
	}

	void preUpdate(FullControl&) {
		++_currentUpdateCount;
		++_totalUpdateCount;
	}

	unsigned entryAttemptCount() const			{ return _entryAttemptCount;	}
	unsigned currentUpdateCount() const			{ return _currentUpdateCount;	}
	unsigned totalUpdateCount() const			{ return _totalUpdateCount;		}

private:
	unsigned _entryAttemptCount	 = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount	 = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Apex
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		auto plan = control.plan();

		plan.change<A, B>();
		plan.change<B, D>();
	}

	void planSucceeded(FullControl& control) {
		REQUIRE(!control.plan());
	}

	void planFailed(FullControl& control) {
		REQUIRE(control.plan());

		control.changeTo<D>();
	}
};

//------------------------------------------------------------------------------

struct A
	: FSM::State
{
	void enter(PlanControl& control) {
		const auto plan = control.plan();

		auto it = plan.begin();
		REQUIRE(it);
		REQUIRE(*it == FSM::Task{ FSM::stateId<A>(), FSM::stateId<B>() });

		++it;
		REQUIRE(it);
		REQUIRE(*it == FSM::Task{ FSM::stateId<B>(), FSM::stateId<D>() });

		++it;
		REQUIRE(!it);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}

	void react(const Interruption&, FullControl& control) {
		control.fail();
	}

	void exitGuard(GuardControl& control) {
		auto plan = control.plan();

		if (!plan) {
			control.cancelPendingTransition();

			plan.change<B, C>();
			plan.change<C>(stateId<C>());
			plan.change(stateId<C>(), stateId<D>());
		}
	}
};

//------------------------------------------------------------------------------

struct C
	: FSM::StateT<Tracked>
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct D
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

////////////////////////////////////////////////////////////////////////////////

void step1(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTRY_GUARD },
		{ FSM::stateId<A>(),		Event::Type::ENTRY_GUARD },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTER },
		{ FSM::stateId<A>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::PRE_UPDATE },
		{ FSM::stateId<A>(),		Event::Type::PRE_UPDATE },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<A>(),		Event::Type::UPDATE },

		{ FSM::stateId<A>(),		Event::Type::TASK_SUCCESS },

		{ FSM::stateId<A>(),		Event::Type::POST_UPDATE },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::POST_UPDATE },

		{ FSM::stateId<A>(),		Event::Type::CHANGE,	FSM::stateId<B>() },

		{ FSM::stateId<A>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<B>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A>(),		Event::Type::EXIT },
		{ FSM::stateId<B>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.react(Interruption{});

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::PRE_REACT },
		{ FSM::stateId<B>(),		Event::Type::PRE_REACT },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::REACT },
		{ FSM::stateId<B>(),		Event::Type::REACT },

		{ FSM::stateId<B>(),		Event::Type::TASK_FAILURE },

		{ FSM::stateId<B>(),		Event::Type::POST_REACT },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::POST_REACT },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::PLAN_FAILED },
		{							Event::Type::CHANGE,	FSM::stateId<D>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<B>(),		Event::Type::CANCEL_PENDING },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::PRE_UPDATE },
		{ FSM::stateId<B>(),		Event::Type::PRE_UPDATE },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<B>(),		Event::Type::UPDATE },

		{ FSM::stateId<B>(),		Event::Type::TASK_SUCCESS },

		{ FSM::stateId<B>(),		Event::Type::POST_UPDATE },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::POST_UPDATE },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B>(),		Event::Type::EXIT },
		{ FSM::stateId<C>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<C>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::PRE_UPDATE },
		{ FSM::stateId<C>(),		Event::Type::PRE_UPDATE },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<C>(),		Event::Type::UPDATE },

		{ FSM::stateId<C>(),		Event::Type::TASK_SUCCESS },

		{ FSM::stateId<C>(),		Event::Type::POST_UPDATE },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::POST_UPDATE },

		{ FSM::stateId<C>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<C>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::REENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<C>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step6(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::PRE_UPDATE },
		{ FSM::stateId<C>(),		Event::Type::PRE_UPDATE },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<C>(),		Event::Type::UPDATE },

		{ FSM::stateId<C>(),		Event::Type::TASK_SUCCESS },

		{ FSM::stateId<C>(),		Event::Type::POST_UPDATE },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::POST_UPDATE },

		{ FSM::stateId<C>(),		Event::Type::CHANGE,	FSM::stateId<D>() },

		{ FSM::stateId<C>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<D>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::EXIT },
		{ FSM::stateId<D>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<D>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step7(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::PRE_UPDATE },
		{ FSM::stateId<D>(),		Event::Type::PRE_UPDATE },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<D>(),		Event::Type::UPDATE },

		{ FSM::stateId<D>(),		Event::Type::TASK_SUCCESS },

		{ FSM::stateId<D>(),		Event::Type::POST_UPDATE },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::POST_UPDATE },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::PLAN_SUCCEEDED },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<D>());
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Plans Verbose") {
	Context _;
	Logger logger;

	{
		FSM::Instance machine{_, &logger};

		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
		step5(machine, logger);
		step6(machine, logger);
		step7(machine, logger);
	}

	logger.assertSequence({
		{ FSM::stateId<D>(),		Event::Type::EXIT },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
