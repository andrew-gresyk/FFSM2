// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_transitions_verbose {

////////////////////////////////////////////////////////////////////////////////

struct Context {};

using Config = ffsm2::Config
					::ContextT<Context>;

using M = ffsm2::MachineT<Config>;

struct Action {};
struct Reaction {};

using Logger = LoggerT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B),
				S(C),
				S(D),
				S(E)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");
static_assert(FSM::stateId<C>() == 2, "");
static_assert(FSM::stateId<D>() == 3, "");
static_assert(FSM::stateId<E>() == 4, "");

////////////////////////////////////////////////////////////////////////////////

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

	unsigned entryAttemptCount()  const			{ return _entryAttemptCount;	}
	unsigned currentUpdateCount() const			{ return _currentUpdateCount;	}
	unsigned totalUpdateCount()	  const			{ return _totalUpdateCount;		}

private:
	unsigned _entryAttemptCount	 = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount	 = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct A
	: FSM::State
{};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<C>();
	}

	void react(const Action&, FullControl& control) {
		control.changeTo<C>();
	}

	using FSM::State::react;
};

//------------------------------------------------------------------------------

struct C
	: FSM::StateT<Tracked>
{
	using Base = FSM::StateT<Tracked>;

	void entryGuard(GuardControl& control) {
		switch (entryAttemptCount()) {
		case 1:
			control.cancelPendingTransition();
			break;

		case 2:
			control.cancelPendingTransition();
			control.changeTo<D>();
			break;

		case 3:
			break;

		default:
			FFSM2_BREAK();
		}
	}
};

//------------------------------------------------------------------------------

struct D
	: FSM::State
{
	void exitGuard(GuardControl& control) {
		control.cancelPendingTransition();
		control.changeTo<C>();
	}
};

//------------------------------------------------------------------------------

struct E
	: FSM::State
{
	//void exitGuard(GuardControl&)												{}
};

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
	machine.changeTo<A>();
	machine.update();

	logger.assertSequence({
		{							Event::Type::CHANGE,	FSM::stateId<A>() },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<A>(),		Event::Type::UPDATE },

		{ FSM::stateId<A>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<A>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A>(),		Event::Type::REENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.changeTo<B>();
	machine.update();

	logger.assertSequence({
		{							Event::Type::CHANGE,	FSM::stateId<B>() },

		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<A>(),		Event::Type::UPDATE },

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

void step4(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::UPDATE },
		{ FSM::stateId<B>(),		Event::Type::UPDATE },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },

		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },
		{ FSM::stateId<C>(),		Event::Type::CANCEL_PENDING },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::REACT },
		{ FSM::stateId<B>(),		Event::Type::REACT },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::CANCEL_PENDING },
		{ FSM::stateId<C>(),		Event::Type::CHANGE,	FSM::stateId<D>() },

		{ FSM::stateId<B>(),		Event::Type::EXIT_GUARD },
		{ FSM::stateId<D>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B>(),		Event::Type::EXIT },
		{ FSM::stateId<B>(),		Event::Type::DESTRUCT },

		{ FSM::stateId<D>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<D>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<D>());
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Transitions Verbose") {
	Context _;
	Logger logger;

	{
		FSM::Instance machine{_, &logger};

		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
		step5(machine, logger);
	}

	logger.assertSequence({
		{ FSM::stateId<D>(),		Event::Type::EXIT },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::EXIT },

		{ FSM::stateId<D>(),		Event::Type::DESTRUCT },
		{ ffsm2::INVALID_STATE_ID,	Event::Type::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
