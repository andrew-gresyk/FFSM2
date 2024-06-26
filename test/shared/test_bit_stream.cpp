// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_SERIALIZATION
#include "../tools.hpp"

namespace test_bit_stream {

////////////////////////////////////////////////////////////////////////////////

using WriteStream  = ffsm2::detail::BitWriteStreamT<45>;
using ReadStream   = ffsm2::detail::BitReadStreamT <45>;
using StreamBuffer = typename WriteStream::Buffer;

//------------------------------------------------------------------------------

TEST_CASE("Shared.BitStream<>") {
	StreamBuffer buffer;

	WriteStream writeStream{buffer};
	writeStream.write< 5>(static_cast<uint8_t >(     27));
	writeStream.write< 4>(static_cast<uint8_t >(     11));
	writeStream.write< 3>(static_cast<uint8_t >(      5));
	writeStream.write<12>(static_cast<uint16_t>(   1472));
	writeStream.write<21>(static_cast<uint32_t>(1000000));
	REQUIRE(writeStream.cursor() == 45);

	ReadStream readStream{buffer};
	REQUIRE(readStream.read< 5>() ==      27);
	REQUIRE(readStream.read< 4>() ==      11);
	REQUIRE(readStream.read< 3>() ==       5);
	REQUIRE(readStream.read<12>() ==    1472);
	REQUIRE(readStream.read<21>() == 1000000);
	REQUIRE(readStream.cursor() == 45);
}

////////////////////////////////////////////////////////////////////////////////

}
