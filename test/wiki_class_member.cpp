// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "wiki_class_member.hpp"

#include <ffsm2/machine.hpp>

#include <doctest/doctest.h>

////////////////////////////////////////////////////////////////////////////////
namespace actor_fsm {

//------------------------------------------------------------------------------

using Config = ffsm2::Config
					::ContextT<Actor&>;

using M = ffsm2::MachineT<Config>;

#define S(s) struct s
using FSM = M::PeerRoot<
				S(Off),
				S(On)
			>;
#undef S

struct Off : FSM::State {};
struct On  : FSM::State {};

FSM::Instance&
fsm(	  Actor::FsmHost& fsmHost) { return *reinterpret_cast<		FSM::Instance*>(&fsmHost); }

const FSM::Instance&
fsm(const Actor::FsmHost& fsmHost) { return *reinterpret_cast<const FSM::Instance*>(&fsmHost); }

//------------------------------------------------------------------------------

}

Actor::Actor() {
	//ffsm2::StaticPrintConstT<alignof(actor_fsm::FSM::Instance)> alignment;
	static_assert(alignof(actor_fsm::FSM::Instance) <= alignof(FsmHost),
				  "Uncomment the line above to find out the alignment of the `FsmHost` needed");

	//ffsm2::StaticPrintConstT<sizeof(actor_fsm::FSM::Instance)> size;
	static_assert(sizeof(actor_fsm::FSM::Instance) <= sizeof(FsmHost),
				  "Uncomment the line above to find out the size of the `FsmHost` needed");

	new (&_fsmHost) actor_fsm::FSM::Instance{*this};
}

Actor::~Actor() {
	ffsm2::destroy(actor_fsm::fsm(_fsmHost));
}

void Actor::turnOn() {
	actor_fsm::fsm(_fsmHost).immediateChangeTo<actor_fsm::On>();
}

bool Actor::isOn() const {
	return actor_fsm::fsm(_fsmHost).isActive<actor_fsm::On>();
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Class Member") {
	Actor actor;
	REQUIRE(actor.isOn() == false);

	actor.turnOn();
	REQUIRE(actor.isOn() == true);
}

////////////////////////////////////////////////////////////////////////////////
