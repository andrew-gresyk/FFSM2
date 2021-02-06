#include <ffsm2/machine.hpp>

//------------------------------------------------------------------------------

struct Context {};

using Config = ffsm2::Config::ContextT<Context*>;

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
