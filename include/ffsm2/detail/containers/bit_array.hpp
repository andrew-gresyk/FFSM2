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

public:
	FFSM2_CONSTEXPR(14) BitArrayT()								  noexcept	{ clear();	}

	FFSM2_CONSTEXPR(14) void clear()							  noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14) bool get  (const TIndex index)		const noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14) void set  (const TIndex index)			  noexcept;

	template <typename TIndex>
	FFSM2_CONSTEXPR(14) void clear(const TIndex index)			  noexcept;

private:
	uint8_t _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <>
class BitArrayT<0> final {
public:
	FFSM2_CONSTEXPR(14)	void clear()							  noexcept	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "bit_array.inl"
