#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
class BitArrayT final {
public:
	using Index	= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, 8);

	using BitArray	= BitArrayT<CAPACITY>;

public:
	FFSM2_CONSTEXPR(14)	BitArrayT()										noexcept	{ clear();	}

	FFSM2_CONSTEXPR(14)	void set()										noexcept;

	FFSM2_CONSTEXPR(14)	void clear()									noexcept;

	FFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	bool get  (const TIndex index)			  const noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	void set  (const TIndex index)					noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14)	void clear(const TIndex index)					noexcept;

	FFSM2_CONSTEXPR(14)	bool operator &  (const BitArray& other)  const noexcept;

	FFSM2_CONSTEXPR(14)	void operator &= (const BitArray& other)		noexcept;

private:
	uint8_t _storage[UNIT_COUNT] {};
};

//------------------------------------------------------------------------------

template <>
class BitArrayT<0> final {
public:
	FFSM2_CONSTEXPR(14)	void clear()									noexcept	{}

	FFSM2_CONSTEXPR(11)	bool empty()							  const noexcept	{ return true;	}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "bit_array.inl"
