#define FFSM2_ENABLE_STATIC_PLANS
#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace test_static_plans {

////////////////////////////////////////////////////////////////////////////////

struct Context {};

using Config = ffsm2::Config
::ContextT<Context>;

using M = ffsm2::MachineT<Config>;

struct Interruption {};

using Logger = LoggerT<Config>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Topology<
					S(A),
					S(B),
					S(C),
					S(D)
				>,
				M::StaticPlan<
					M::TaskLink<A, B>,
					M::TaskLink<B, C>,
					M::TaskLink<C, D>
				>
			>;

#undef S

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");
static_assert(FSM::stateId<C>() == 2, "");
static_assert(FSM::stateId<D>() == 3, "");

//------------------------------------------------------------------------------

class Tracked
	: public FSM::Injection
{
public:
	void preEntryGuard(Context&) {
		++_entryAttemptCount;
		_currentUpdateCount = 0;
	}

	void preUpdate(Context&) {
		++_currentUpdateCount;
		++_totalUpdateCount;
	}

	void preExitGuard(Context&) {
		++_exitAttemptCount;
	}

	unsigned entryAttemptCount() const			{ return _entryAttemptCount;	}
	unsigned currentUpdateCount() const			{ return _currentUpdateCount;	}
	unsigned totalUpdateCount() const			{ return _totalUpdateCount;		}
	unsigned exitAttemptCount() const			{ return _exitAttemptCount;		}

private:
	unsigned _entryAttemptCount	 = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount	 = 0;
	unsigned _exitAttemptCount	 = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Apex
	: FSM::State
{};

//------------------------------------------------------------------------------

struct A
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct B
	: FSM::StateT<Tracked>
{
	void update(FullControl& control) {
		control.succeed();
	}

	void react(const Interruption&, FullControl& control) {
		control.changeTo<D>();
	}

	void exitGuard(GuardControl& control) {
		if (exitAttemptCount() == 1)
			control.cancelPendingTransition();
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
{};

////////////////////////////////////////////////////////////////////////////////

void step1(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTRY_GUARD },
		{ FSM::stateId<A>(),		Event::Type::ENTRY_GUARD },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::CONSTRUCT },
		{ FSM::stateId<A>(),		Event::Type::CONSTRUCT },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTER },
		{ FSM::stateId<A>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<A>(),		Event::Type::UPDATE },

		{ FSM::stateId<A>(),		Event::Type::TASK_SUCCESS },
		{ FSM::stateId<A>(),		Event::Type::CHANGE,	FSM::stateId<B>() },

		{ FSM::stateId<A>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<B>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A>(),		Event::Type::EXIT },
		{ FSM::stateId<A>(),		Event::Type::DESTRUCT },

		{ FSM::stateId<B>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<B>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.react(Interruption{});

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::REACT },
		{ FSM::stateId<B>(),		Event::Type::REACT },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<D>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<B>(),		Event::Type::CANCEL_PENDING },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<B>(),		Event::Type::UPDATE },

		{ FSM::stateId<B>(),		Event::Type::TASK_SUCCESS },
		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B>(),		Event::Type::EXIT },
		{ FSM::stateId<B>(),		Event::Type::DESTRUCT },

		{ FSM::stateId<C>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<C>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<C>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<C>(),		Event::Type::UPDATE },

		{ FSM::stateId<C>(),		Event::Type::TASK_SUCCESS },
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
		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<C>(),		Event::Type::UPDATE },

		{ FSM::stateId<C>(),		Event::Type::TASK_SUCCESS },
		{ FSM::stateId<C>(),		Event::Type::CHANGE,	FSM::stateId<D>() },

		{ FSM::stateId<C>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<D>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::EXIT },
		{ FSM::stateId<C>(),		Event::Type::DESTRUCT },

		{ FSM::stateId<D>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<D>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<D>());
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Static Plans", "[machine]") {
	Context _;
	LoggerT<Config> logger;

	{
		FSM::Instance machine{_, &logger};

		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
		//step5(machine, logger);
		step6(machine, logger);
	}

	//logger.assertSequence({
	//	{ FSM::stateId<D>(),		Event::Type::EXIT },
	//	{ ffsm2::INVALID_STATE_ID,	Event::Type::EXIT },
	//
	//	{ FSM::stateId<D>(),		Event::Type::DESTRUCT },
	//	{ ffsm2::INVALID_STATE_ID,	Event::Type::DESTRUCT },
	//});
}

////////////////////////////////////////////////////////////////////////////////

}

#endif
