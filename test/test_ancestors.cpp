// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#define FFSM2_DISABLE_TYPEINDEX
#include <ffsm2/machine.hpp>

#include <doctest/doctest.h>

#include <algorithm> // std::max()
#include <vector>

namespace test_ancestors {

//------------------------------------------------------------------------------

struct Event {
	enum class Type {
		ENTRY_GUARD,

		ENTER,
		PRE_REENTER,

		PRE_UPDATE,
		UPDATE,
		POST_UPDATE,

		PRE_REACT,
		REACT,
		POST_REACT,

		EXIT_GUARD,

		EXIT,

		COUNT
	};

	Event(const ffsm2::StateID origin_,
		  const Type type_) noexcept
		: origin{origin_}
		, type{type_}
	{}

	Event(const Type type_ = Type::COUNT) noexcept
		: type{type_}
	{}

	ffsm2::StateID origin = ffsm2::INVALID_STATE_ID;
	Type type;
};

using Events = std::vector<Event>;

//------------------------------------------------------------------------------

void
assertSequence(Events& history,
			   const Events& reference) noexcept
{
	const auto count = std::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history.size()); //-V521
		REQUIRE(i < reference.size()); //-V521

		if (i < history.size() &&
			i < reference.size())
		{
			REQUIRE(history[i].type	  == reference[i].type); //-V521
			REQUIRE(history[i].origin == reference[i].origin); //-V521
		}
	}

	history.clear();
}

////////////////////////////////////////////////////////////////////////////////

using Config = ffsm2::Config
					::ContextT<Events&>
					::ManualActivation;

using M = ffsm2::MachineT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(R),
				S(A),
				S(B)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<R>() == ffsm2::INVALID_STATE_ID, "");
static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct AncestorT
	: FSM::State
{
	void entryGuard		  (GuardControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::ENTRY_GUARD);	}

	void enter			  ( PlanControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::ENTER);			}
	void reenter		  ( PlanControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::PRE_REENTER);	}

	void preUpdate		  ( FullControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::PRE_UPDATE);		}
	void update			  ( FullControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::UPDATE);			}
	void postUpdate		  ( FullControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::POST_UPDATE);	}

	void preReact		  (const Event&,
						    FullControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::PRE_REACT);		}

	void react			  (const Event&,
						    FullControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::REACT);			}

	void postReact		  (const Event&,
						    FullControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::POST_REACT);		}

	void exitGuard		  (GuardControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::EXIT_GUARD);		}

	void exit			  ( PlanControl& control)	{ control._().emplace_back(FSM::stateId<T>(), Event::Type::EXIT);			}
};

//------------------------------------------------------------------------------

struct R
	: FSM::AncestorsT<AncestorT<R>>
{
	void entryGuard(GuardControl& control) {
		control.changeTo<B>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A
	: FSM::AncestorsT<AncestorT<A>>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B
	: FSM::AncestorsT<AncestorT<B>>
{};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Ancestors") {
	Events events;

	{
		FSM::Instance machine{events};

		assertSequence(events, {});

		machine.enter();
		assertSequence(events, {
			{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<A>(),		Event::Type::ENTRY_GUARD },
			{ ffsm2::INVALID_STATE_ID,	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<B>(),		Event::Type::ENTRY_GUARD },
			{ FSM::stateId<R>(),		Event::Type::ENTER },
			{ FSM::stateId<B>(),		Event::Type::ENTER },
		});
		REQUIRE(machine.activeStateId() == FSM::stateId<B>());

		machine.update();
		assertSequence(events, {
			{ FSM::stateId<R>(),		Event::Type::PRE_UPDATE },
			{ FSM::stateId<B>(),		Event::Type::PRE_UPDATE },
			{ FSM::stateId<R>(),		Event::Type::UPDATE },
			{ FSM::stateId<B>(),		Event::Type::UPDATE },
			{ FSM::stateId<B>(),		Event::Type::POST_UPDATE },
			{ FSM::stateId<R>(),		Event::Type::POST_UPDATE },
		});

		machine.react(Event{});
		assertSequence(events, {
			{ FSM::stateId<R>(),		Event::Type::PRE_REACT },
			{ FSM::stateId<B>(),		Event::Type::PRE_REACT },
			{ FSM::stateId<R>(),		Event::Type::REACT },
			{ FSM::stateId<B>(),		Event::Type::REACT },
			{ FSM::stateId<B>(),		Event::Type::POST_REACT },
			{ FSM::stateId<R>(),		Event::Type::POST_REACT },
		});

		machine.exit();
		assertSequence(events, {
			{ FSM::stateId<B>(),		Event::Type::EXIT },
			{ FSM::stateId<R>(),		Event::Type::EXIT },
		});
		REQUIRE(machine.activeStateId() == ffsm2::INVALID_STATE_ID);
	}

	assertSequence(events, {});
}

////////////////////////////////////////////////////////////////////////////////

}
