#if FFSM2_SERIALIZATION_AVAILABLE()

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

	FFSM2_CONSTEXPR(14)	void clear()										  noexcept	{ fill(_data, 0);	}

	FFSM2_CONSTEXPR(14)		  Data& data()									  noexcept	{ return _data;		}
	FFSM2_CONSTEXPR(11)	const Data& data()								const noexcept	{ return _data;		}

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
	FFSM2_CONSTEXPR(14)	explicit BitWriteStreamT(Buffer& buffer,
												 const Long cursor = 0)		  noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{
		_buffer.clear();
	}

	template <Short NBitWidth>
	FFSM2_CONSTEXPR(14)	void write(const UBitWidth<NBitWidth> item)			  noexcept;

	FFSM2_CONSTEXPR(11)	Long cursor()									const noexcept	{ return _cursor;	}

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
	FFSM2_CONSTEXPR(11)	explicit BitReadStreamT(const Buffer& buffer,
												const Long cursor = 0)		  noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{}

	template <Short NBitWidth>
	FFSM2_CONSTEXPR(14)	UBitWidth<NBitWidth> read()							  noexcept;

	FFSM2_CONSTEXPR(11)	Long cursor()									const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_stream.inl"

#endif
