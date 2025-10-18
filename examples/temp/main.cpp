// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#include <ffsm2/machine.hpp>

//------------------------------------------------------------------------------

#include <variant>

struct Context {};
using Payload = std::variant<int>; // This doesn't work

using Config = ffsm2::Config::ContextT<Context*>::PayloadT<Payload>;

using M = ffsm2::MachineT<Config>;

#define S(s) struct s

using FSM = M::PeerRoot<S(A), S(B)>;

#undef S

struct A
	: FSM::State
{};

struct B
	: FSM::State
{};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	Context* const empty = nullptr;
	FSM::Instance machine{empty};

	return 0;
}

//------------------------------------------------------------------------------
