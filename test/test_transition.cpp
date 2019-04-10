#include "test_transition.hpp"

#include <catch2/catch.hpp>

using namespace test;

// introspection helper
template <typename TState>
void assertActive(const FSM& fsm) {
	REQUIRE(std::is_same_v<TState, Off>		   != !fsm.isActive<Off>());
	REQUIRE(std::is_same_v<TState, Red>		   != !fsm.isActive<Red>());
	REQUIRE(std::is_same_v<TState, YellowUp>   != !fsm.isActive<YellowUp>());
	REQUIRE(std::is_same_v<TState, YellowDown> != !fsm.isActive<YellowDown>());
	REQUIRE(std::is_same_v<TState, Green>	   != !fsm.isActive<Green>());
}

// basic transition unit test
TEST_CASE("Transition test", "[ffsm]") {
	Context _;

	FSM fsm(_);
	assertActive<Off>(fsm);

	fsm.changeTo<Red>();
	assertActive<Red>(fsm);

	fsm.update();
	assertActive<YellowDown>(fsm);

	fsm.update();
	assertActive<Green>(fsm);

	fsm.update();
	assertActive<YellowUp>(fsm);

	fsm.update();
	assertActive<Red>(fsm);

	fsm.update();
	assertActive<YellowDown>(fsm);
}
