// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_TRANSITION_HISTORY
#define FFSM2_DISABLE_TYPEINDEX
#include "tools.hpp"

namespace test_transitions {

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

	unsigned entryAttemptCount() const			{ return _entryAttemptCount;	}
	unsigned currentUpdateCount() const			{ return _currentUpdateCount;	}
	unsigned totalUpdateCount() const			{ return _totalUpdateCount;		}

private:
	unsigned _entryAttemptCount	 = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount	 = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct A
	: FSM::State
{
	void enter(PlanControl&)													{}
	void reenter(PlanControl&)													{}
	void update(FullControl&)													{}
	void exit(PlanControl&)														{}
};

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

	void react(const Reaction&, FullControl&)									{}

	using Base::react;
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

const Types all = {
	FSM::stateId<A>(),
	FSM::stateId<B>(),
	FSM::stateId<C>(),
	FSM::stateId<D>(),
	FSM::stateId<E>(),
};

//------------------------------------------------------------------------------

void step1(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ FSM::stateId<A>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<A>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());

	REQUIRE(!machine.previousTransition());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.changeTo<A>();
	machine.update();

	logger.assertSequence({
		{							Event::Type::CHANGE,	FSM::stateId<A>() },

		{ FSM::stateId<A>(),		Event::Type::UPDATE },

		{ FSM::stateId<A>(),		Event::Type::REENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());

	REQUIRE(machine.previousTransition() == M::Transition{FSM::stateId<A>()});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.changeTo<B>();
	machine.update();

	logger.assertSequence({
		{							Event::Type::CHANGE,	FSM::stateId<B>() },

		{ FSM::stateId<A>(),		Event::Type::UPDATE },

		{ FSM::stateId<A>(),		Event::Type::EXIT },
		{ FSM::stateId<A>(),		Event::Type::DESTRUCT },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());

	REQUIRE(machine.previousTransition() == M::Transition{FSM::stateId<B>()});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<B>(),		Event::Type::UPDATE },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },
		{ FSM::stateId<C>(),		Event::Type::CANCEL_PENDING },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());

	REQUIRE(!machine.previousTransition());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::REACT },
		{ FSM::stateId<B>(),		Event::Type::REACT },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::CANCEL_PENDING },
		{ FSM::stateId<C>(),		Event::Type::CHANGE,	FSM::stateId<D>() },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<D>());

	REQUIRE(machine.previousTransition() == M::Transition{FSM::stateId<C>(), FSM::stateId<D>()});
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Transitions") {
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

	logger.assertSequence({});
}

////////////////////////////////////////////////////////////////////////////////

}
