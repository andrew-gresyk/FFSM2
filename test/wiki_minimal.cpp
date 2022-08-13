// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#include <doctest/doctest.h>
#include <ffsm2/machine.hpp>

struct Context {
	bool on = false;
};

using Config = ffsm2::Config
					::ContextT<Context&>;

using M = ffsm2::MachineT<Config>;

using FSM = M::PeerRoot<
				struct Off,
				struct On
			>;

struct Off
	: FSM::State
{
	void enter(PlanControl& control) {
		control.context().on = false;
	}
};

struct On
	: FSM::State
{
	void enter(PlanControl& control) {
		control.context().on = true;
	}
};

TEST_CASE("Wiki.Minimal Example") {
	Context context;
	FSM::Instance machine{context};

	machine.immediateChangeTo<On>();

	REQUIRE(context.on == true);
}
