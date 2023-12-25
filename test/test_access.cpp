// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_access {

////////////////////////////////////////////////////////////////////////////////

using M = ffsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(R),
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

struct R : FSM::State { const ffsm2::StateID id = FSM::stateId<R>(); };
struct A : FSM::State { const ffsm2::StateID id = FSM::stateId<A>(); };
struct B : FSM::State { const ffsm2::StateID id = FSM::stateId<B>(); };
struct C : FSM::State { const ffsm2::StateID id = FSM::stateId<C>(); };
struct D : FSM::State { const ffsm2::StateID id = FSM::stateId<D>(); };

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Access") {
	FSM::Instance machine;

	REQUIRE(machine.access<R>().id == FSM::stateId<R>());
	REQUIRE(machine.access<A>().id == FSM::stateId<A>());
	REQUIRE(machine.access<B>().id == FSM::stateId<B>());
	REQUIRE(machine.access<C>().id == FSM::stateId<C>());
	REQUIRE(machine.access<D>().id == FSM::stateId<D>());
}

////////////////////////////////////////////////////////////////////////////////

}
