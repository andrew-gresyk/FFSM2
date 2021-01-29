#ifdef FFSM2_ENABLE_SERIALIZATION

namespace ffsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <Long>
class BitWriteStreamT;

template <Long>
class BitReadStreamT;

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class StreamBufferT {
	template <Long>
	friend class BitWriteStreamT;

	template <Long>
	friend class BitReadStreamT;

public:
	static constexpr Long BIT_CAPACITY	= NBitCapacity;
	static constexpr Long BYTE_COUNT	= contain(BIT_CAPACITY, 8u);

	using Data = uint8_t[BYTE_COUNT];

	FFSM2_INLINE void clear()										  noexcept;

	FFSM2_INLINE	   Data& data()									  noexcept	{ return _data;		}
	FFSM2_INLINE const Data& data()								const noexcept	{ return _data;		}

private:
	Data _data;
};

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class BitWriteStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	FFSM2_INLINE explicit BitWriteStreamT(Buffer& buffer,
										  const Long cursor = 0)	  noexcept;

	template <Short NBitWidth>
	FFSM2_INLINE void write(const UnsignedBitWidth<NBitWidth> item)	  noexcept;

	FFSM2_INLINE Long cursor()									const noexcept	{ return _cursor;	}

private:
	Buffer& _buffer;

	Long _cursor = 0;
};

//------------------------------------------------------------------------------

template <Long NBitCapacity>
class BitReadStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	FFSM2_INLINE explicit BitReadStreamT(const Buffer& buffer,
										 const Long cursor = 0)		  noexcept;

	template <Short NBitWidth>
	FFSM2_INLINE UnsignedBitWidth<NBitWidth> read()					  noexcept;

	FFSM2_INLINE Long cursor()									const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_stream.inl"

#endif
