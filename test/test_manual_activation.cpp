// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#define FFSM2_DISABLE_TYPEINDEX
#include "tools.hpp"

namespace test_manual_activation {

////////////////////////////////////////////////////////////////////////////////

using Config = ffsm2::Config
					::ManualActivation;

using M = ffsm2::MachineT<Config>;

using Logger = LoggerT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");

////////////////////////////////////////////////////////////////////////////////

struct A
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		control.changeTo<B>();
	}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Manual Activation") {
	Logger logger;

	{
		FSM::Instance machine{&logger};

		logger.assertSequence({});

		machine.enter();

		logger.assertSequence({
			{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<A>(),		Event::Type::ENTRY_GUARD },

			{ FSM::stateId<A>(),		Event::Type::CHANGE,	FSM::stateId<B>() },

			{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<B>(),		Event::Type::ENTRY_GUARD },

			{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTER },
			{ FSM::stateId<B>(),		Event::Type::ENTER },
		});

		REQUIRE(machine.activeStateId() == FSM::stateId<B>());

		machine.exit();

		logger.assertSequence({
			{ FSM::stateId<B>(),		Event::Type::EXIT },
			{ ffsm2::INVALID_STATE_ID,	Event::Type::EXIT },
		});

		REQUIRE(machine.activeStateId() == ffsm2::INVALID_STATE_ID);
	}

	logger.assertSequence({});
}

////////////////////////////////////////////////////////////////////////////////

}
