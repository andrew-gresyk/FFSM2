#include <ffsm2/machine.hpp>
#include <catch2/catch.hpp>

namespace test_contexts {

//------------------------------------------------------------------------------

using Context = int;

////////////////////////////////////////////////////////////////////////////////

namespace value_context {

using M = ffsm2::MachineT<ffsm2::Config::ContextT<Context>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace reference_context {

using M = ffsm2::MachineT<ffsm2::Config::ContextT<Context&>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace pointer_context {

using M = ffsm2::MachineT<ffsm2::Config::ContextT<Context*>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace no_context {

using M = ffsm2::Machine;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Contexts", "[machine]") {
	Context _ = 7;

	// context is a value
	{
		using Instance = value_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context>::value, "");

		//Instance defaultConstructed;

		const Instance constant{_};
		REQUIRE(constant.context() == 7);

		Instance machine{_};
		REQUIRE(machine.context() == 7);

		machine.setContext(_);
		machine.setContext(8);
	}

	// context is a reference
	{
		using Instance = reference_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context&>::value, "");

		const Instance constant{_};
		REQUIRE(constant.context() == 7);

		Instance machine{_};
		REQUIRE(machine.context() == 7);

		machine.setContext(_);
	}

	// context is a pointer
	{
		using Instance = pointer_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context*>::value, "");

		pointer_context::FSM::Instance defaultConstructed;

		const Instance constant{&_};
		REQUIRE(*constant.context() == 7);

		Instance machine{&_};
		REQUIRE(*machine.context() == 7);

		machine.setContext(&_);
	}

	// empty context
	{
		no_context::FSM::Instance defaultConstructed;
	}
}

////////////////////////////////////////////////////////////////////////////////

}
