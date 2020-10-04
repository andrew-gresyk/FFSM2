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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B),
				S(C),
				S(D)
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
	//void entryGuard(GuardControl&)											{}
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	//void react(const Action&, FullControl&)									{}
	void exit(PlanControl&)														{}
	//void exitGuard(GuardControl&)												{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	//void entryGuard(GuardControl&)											{}
	//void enter(PlanControl&)													{}

	void update(FullControl& control) {
		control.changeTo<C>();
	}

	void react(const Action&, FullControl& control) {
		control.changeTo<C>();
	}

	using FSM::State::react;

	//void exit(PlanControl&)													{}
	//void exitGuard(GuardControl&)												{}
};

//------------------------------------------------------------------------------

struct C
	: FSM::StateT<Tracked>
{
	using Base = FSM::StateT<Tracked>;

	void entryGuard(GuardControl& control) {
		if (entryAttemptCount() == 1)
			control.cancelPendingTransition();
	}

	//void enter(PlanControl&)													{}
	//void update(FullControl&)													{}

	void react(const Reaction&, FullControl&)									{}

	using Base::react;

	//void exit(PlanControl&)													{}
	void exitGuard(GuardControl&)												{}
};

//------------------------------------------------------------------------------

struct D
	: FSM::State
{
	//void entryGuard(GuardControl&)											{}
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	//void react(const Action&, FullControl&)									{}
	//void exit(PlanControl&)													{}
	//void exitGuard(GuardControl&)												{}
};

////////////////////////////////////////////////////////////////////////////////

void step1(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ FSM::stateId<A>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<A>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<A>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.changeTo<B>();
	machine.update();

	logger.assertSequence({
		{							Event::Type::CHANGE, FSM::stateId<B>() },

		{ FSM::stateId<A>(),		Event::Type::UPDATE },

		{ FSM::stateId<A>(),		Event::Type::EXIT },
		{ FSM::stateId<A>(),		Event::Type::DESTRUCT },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<B>(),		Event::Type::UPDATE },

		{ FSM::stateId<B>(),		Event::Type::CHANGE, FSM::stateId<C>() },

		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },
		{ FSM::stateId<C>(),		Event::Type::CANCEL_PENDING },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<B>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	Event::Type::REACT },
		{ FSM::stateId<B>(),		Event::Type::REACT },

		{ FSM::stateId<B>(),		Event::Type::CHANGE, FSM::stateId<C>() },

		{ FSM::stateId<C>(),		Event::Type::ENTRY_GUARD },

		{ FSM::stateId<C>(),		Event::Type::CONSTRUCT },
		{ FSM::stateId<C>(),		Event::Type::ENTER },
	});

	REQUIRE(machine.activeStateId() == FSM::stateId<C>());
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Transitions", "[machine]") {
	Context _;
	LoggerT<Config> logger;

	{
		FSM::Instance machine{_, &logger};

		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
	}

	logger.assertSequence({
		{ FSM::stateId<C>(),		Event::Type::EXIT },
		{ FSM::stateId<C>(),		Event::Type::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
