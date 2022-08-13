// FFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_TRANSITION_HISTORY
#include "tools.hpp"

namespace test_replication {

////////////////////////////////////////////////////////////////////////////////

using Config = ffsm2::Config
					::ManualActivation;

using M = ffsm2::MachineT<Config>;

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

static_assert(FSM::stateId<A>() ==  0, "");
static_assert(FSM::stateId<B>() ==  1, "");
static_assert(FSM::stateId<C>() ==  2, "");
static_assert(FSM::stateId<D>() ==  3, "");
static_assert(FSM::stateId<E>() ==  4, "");

//------------------------------------------------------------------------------

struct A : FSM::State {};
struct B : FSM::State {};
struct C : FSM::State {};
struct D : FSM::State {};
struct E : FSM::State {};

//------------------------------------------------------------------------------

const Types all = {
	FSM::stateId<A>(),
	FSM::stateId<B>(),
	FSM::stateId<C>(),
	FSM::stateId<D>(),
	FSM::stateId<E>(),
};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Replication") {
	Logger logger;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FSM::Instance authority {&logger};
	FSM::Instance replicated{&logger};
	{
		REQUIRE(authority .activeStateId() == ffsm2::INVALID_STATE_ID);
		REQUIRE(replicated.activeStateId() == ffsm2::INVALID_STATE_ID);

		REQUIRE(!authority .previousTransition());
		REQUIRE(!replicated.previousTransition());
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	authority .enter();
	REQUIRE(authority.activeStateId() == FSM::stateId<A>());
	REQUIRE(!authority.previousTransition());

	authority.immediateChangeTo<B>();
	REQUIRE(authority.activeStateId() == FSM::stateId<B>());

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	replicated.replayEnter(authority.previousTransition().destination);
	REQUIRE(replicated.activeStateId() == FSM::stateId<B>());
	REQUIRE(replicated.previousTransition() == FSM::Transition{FSM::stateId<B>()});

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	authority.immediateChangeTo<C>();
	REQUIRE(authority.activeStateId() == FSM::stateId<C>());

	REQUIRE(replicated.replayTransition(authority.previousTransition().destination));
	REQUIRE(replicated.activeStateId() == FSM::stateId<C>());

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

////////////////////////////////////////////////////////////////////////////////

}
