// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "wiki_class_member.hpp"

#include <ffsm2/machine.hpp>

#include <doctest/doctest.h>

namespace actor_fsm {

using Config = ffsm2::Config
					::ContextT<Actor::Context&>;

using M = ffsm2::MachineT<Config>;

#define S(s) struct s
using FSM = M::PeerRoot<
	S(Off),
	S(On)
>;
#undef S

struct Off : FSM::State { /* .. */ };
struct On  : FSM::State { /* .. */ };

FSM::Instance&
fsm(Actor::FsmHost& fsmHost) {
	return *reinterpret_cast<FSM::Instance*>(&fsmHost);
}

}

Actor::Actor() {
	//ffsm2::StaticPrintConstT<sizeof(actor_fsm::FSM::Instance)> dummy;
	static_assert(sizeof(actor_fsm::FSM::Instance) <= sizeof(FsmHost), "");

	new (&_fsmHost) actor_fsm::FSM::Instance{_context};
}

Actor::~Actor() {
	actor_fsm::fsm(_fsmHost).~RC_();
}
