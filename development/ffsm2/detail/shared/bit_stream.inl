#if FFSM2_SERIALIZATION_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
FFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator == (const StreamBuffer& buffer) const noexcept {
	for (Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long NBitCapacity>
FFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator != (const StreamBuffer& buffer) const noexcept {
	for (Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
template <Short NBitWidth>
FFSM2_CONSTEXPR(14)
void
BitWriteStreamT<NBitCapacity>::write(const UBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
template <Short NBitWidth>
FFSM2_CONSTEXPR(14)
UBitWidth<NBitWidth>
BitReadStreamT<NBitCapacity>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	FFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item item = 0;

	for (Short itemCursor = 0, itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		const uint8_t& byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
		const Short byteChunkMask	= (1 << byteChunkWidth) - 1;

		const Item	byteChunk		= (byte >> byteChunkStart) & byteChunkMask;
		const Item	itemChunk		= byteChunk << itemCursor;

		item		|= itemChunk;
		itemCursor	+= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}

	return item;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
