// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "tools.hpp"

namespace test_transitions_payloads {

////////////////////////////////////////////////////////////////////////////////

struct Context {};

struct Payload {
	int value;
};

using Config = ffsm2::Config
					::ContextT<Context>
					::PayloadT<Payload>;

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

	unsigned entryAttemptCount()  const			{ return _entryAttemptCount;	}
	unsigned currentUpdateCount() const			{ return _currentUpdateCount;	}
	unsigned totalUpdateCount()   const			{ return _totalUpdateCount;		}

private:
	unsigned _entryAttemptCount	 = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount	 = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct A
	: FSM::State
{
	void enter	(PlanControl&)													{}
	void reenter(PlanControl&)													{}
	void update	(FullControl&)													{}
	void exit	(PlanControl&)													{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		REQUIRE(control.pendingTransition().payload());
		REQUIRE(control.pendingTransition().payload()->value == 5);

		REQUIRE(control.currentTransition().payload() == nullptr);
	}

	void enter(PlanControl& control) {
		REQUIRE(control.currentTransition().payload());
		REQUIRE(control.currentTransition().payload()->value == 5);
	}

	void update(FullControl& control) {
		control.changeWith<C>(Payload{1});
	}

	void react(const Action&, FullControl& control) {
		control.changeWith<C>(Payload{2});
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
			REQUIRE(control.pendingTransition().payload());
			REQUIRE(control.pendingTransition().payload()->value == 1);

			control.cancelPendingTransition();
			break;

		case 2:
			REQUIRE(control.pendingTransition().payload());
			REQUIRE(control.pendingTransition().payload()->value == 2);

			control.cancelPendingTransition();
			control.changeWith<D>(Payload{3});
			break;

		default:
			FFSM2_BREAK();
		}
	}

	void enter(PlanControl& control) {
		switch (entryAttemptCount()) {
		case 1:
			REQUIRE(control.currentTransition().payload());
			REQUIRE(control.currentTransition().payload()->value == 1);
			break;

		case 2:
			REQUIRE(control.currentTransition().payload());
			REQUIRE(control.currentTransition().payload()->value == 2);
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
	void entryGuard(GuardControl& control) {
		REQUIRE(control.pendingTransition().payload());
		REQUIRE(control.pendingTransition().payload()->value == 3);

		REQUIRE(control.currentTransition().payload() == nullptr);
	}

	void enter(PlanControl& control) {
		REQUIRE(control.currentTransition().payload());
		REQUIRE(control.currentTransition().payload()->value == 3);
	}
};

////////////////////////////////////////////////////////////////////////////////

void step1(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ FSM::stateId<A>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.immediateChangeWith<A>(Payload{4});

	logger.assertSequence({
		{							Event::Type::CHANGE,	FSM::stateId<A>() },

		{ FSM::stateId<A>(),		Event::Type::REENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.immediateChangeWith<B>(Payload{5});

	logger.assertSequence({
		{							Event::Type::CHANGE,	FSM::stateId<B>() },

		{ FSM::stateId<B>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A>(),		Event::Type::EXIT },
		{ FSM::stateId<B>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
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
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ FSM::stateId<B>(),		Event::Type::PRE_REACT },
		{ FSM::stateId<B>(),		Event::Type::REACT },

		{ FSM::stateId<B>(),		Event::Type::CHANGE,	FSM::stateId<C>() },

		{ FSM::stateId<B>(),		Event::Type::POST_REACT },

		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::CANCEL_PENDING },
		{ FSM::stateId<C>(),		Event::Type::CHANGE,	FSM::stateId<D>() },

		{ FSM::stateId<D>(),		Event::Type::ENTRY_GUARD },
		{ FSM::stateId<D>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<D>());
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Transition Payloads") {
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
