#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_transitions_verbose {

////////////////////////////////////////////////////////////////////////////////

using Config = ffsm2::Config::ContextT<float>;

using M = ffsm2::MachineT<Config>;

struct Action {};
struct Reaction {};

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
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	//void exit(PlanControl&)													{}
	//void exitGuard(GuardControl&)												{}
};

//------------------------------------------------------------------------------

struct C
	: FSM::State
{
	void entryGuard(GuardControl&)												{}
	//void enter(PlanControl&)													{}
	//void update(FullControl&)													{}
	void react(const Reaction&, FullControl&)									{}
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

TEST_CASE("FSM.Transitions Verbose", "[machine]") {
	float _ = 0.0f;
	LoggerT<Config> logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{_, &logger};
		{
			logger.assertSequence({
				{ ffsm2::INVALID_STATE_ID,	Event::ENTRY_GUARD },
				{ FSM::stateId<A>(),		Event::ENTRY_GUARD },

				{ ffsm2::INVALID_STATE_ID,	Event::CONSTRUCT },
				{ FSM::stateId<A>(),		Event::CONSTRUCT },

				{ ffsm2::INVALID_STATE_ID,	Event::ENTER },
				{ FSM::stateId<A>(),		Event::ENTER },
			});

			REQUIRE(machine.activeStateId() == FSM::stateId<A>());
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.changeTo<B>();
		machine.update();
		{
			logger.assertSequence({
				{							Event::CHANGE, FSM::stateId<B>() },

				{ ffsm2::INVALID_STATE_ID,	Event::UPDATE },
				{ FSM::stateId<A>(),		Event::UPDATE },

				{ FSM::stateId<A>(),		Event::EXIT_GUARD },
				{ FSM::stateId<B>(),		Event::ENTRY_GUARD },

				{ FSM::stateId<A>(),		Event::EXIT },
				{ FSM::stateId<A>(),		Event::DESTRUCT },

				{ FSM::stateId<B>(),		Event::CONSTRUCT },
				{ FSM::stateId<B>(),		Event::ENTER },
			});

			REQUIRE(machine.activeStateId() == FSM::stateId<B>());
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<B>(),		Event::EXIT },
		{ ffsm2::INVALID_STATE_ID,	Event::EXIT },

		{ FSM::stateId<B>(),		Event::DESTRUCT },
		{ ffsm2::INVALID_STATE_ID,	Event::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
