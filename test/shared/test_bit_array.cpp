// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_PLANS
#include "../tools.hpp"

namespace test_bit_array {

////////////////////////////////////////////////////////////////////////////////

using BitArray = ffsm2::detail::BitArrayT<ffsm2::Short, 12>;

TEST_CASE("Shared.BitArrayT<>") {
	BitArray bits;

	bits.clear();

	WHEN("Dynamic methods") {
		REQUIRE(!bits.get( 0));
		REQUIRE(!bits.get( 1));
		REQUIRE(!bits.get( 2));
		REQUIRE(!bits.get( 3));
		REQUIRE(!bits.get( 4));
		REQUIRE(!bits.get( 5));
		REQUIRE(!bits.get( 6));
		REQUIRE(!bits.get( 7));
		REQUIRE(!bits.get( 8));
		REQUIRE(!bits.get( 9));
		REQUIRE(!bits.get(10));
		REQUIRE(!bits.get(11));

		bits.set( 0);
		bits.set( 4);
		bits.set( 5);
		bits.set( 8);
		bits.set(11);

		REQUIRE( bits.get( 0));
		REQUIRE(!bits.get( 1));
		REQUIRE(!bits.get( 2));
		REQUIRE(!bits.get( 3));
		REQUIRE( bits.get( 4));
		REQUIRE( bits.get( 5));
		REQUIRE(!bits.get( 6));
		REQUIRE(!bits.get( 7));
		REQUIRE( bits.get( 8));
		REQUIRE(!bits.get( 9));
		REQUIRE(!bits.get(10));
		REQUIRE( bits.get(11));

		bits.clear( 4);
		bits.clear( 5);
		bits.clear( 8);
		bits.clear( 9);

		REQUIRE( bits.get( 0));
		REQUIRE(!bits.get( 1));
		REQUIRE(!bits.get( 2));
		REQUIRE(!bits.get( 3));
		REQUIRE(!bits.get( 4));
		REQUIRE(!bits.get( 5));
		REQUIRE(!bits.get( 6));
		REQUIRE(!bits.get( 7));
		REQUIRE(!bits.get( 8));
		REQUIRE(!bits.get( 9));
		REQUIRE(!bits.get(10));
		REQUIRE( bits.get(11));
	}
}

////////////////////////////////////////////////////////////////////////////////

}
