#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	inline Units(Short unit_  = INVALID_SHORT,
				 Short width_ = INVALID_SHORT) noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

//------------------------------------------------------------------------------

template <typename TIndex, Short NCapacity>
class BitArrayT final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_WIDTH = sizeof(Unit) * 8;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, UNIT_WIDTH);

public:
	BitArrayT() noexcept {
		clear();
	}

	FFSM2_INLINE void clear()								  noexcept;

	FFSM2_INLINE bool get  (const Index index)			const noexcept;
	FFSM2_INLINE void set  (const Index index)				  noexcept;
	FFSM2_INLINE void clear(const Index index)				  noexcept;

private:
	Unit _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArrayT<TIndex, 0> final {
public:
	FFSM2_INLINE void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"

#endif
