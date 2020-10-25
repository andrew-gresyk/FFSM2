namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	constexpr Units(Short unit_  = INVALID_SHORT,
					Short width_ = INVALID_SHORT) noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

//------------------------------------------------------------------------------

template <typename TIndex, Short NCapacity>
class BitArray final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index CAPACITY	= NCapacity;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, Short>
		friend class BitArray;

	private:
		constexpr explicit Bits(Unit* const storage,
								const Index width) noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		constexpr explicit operator bool() const noexcept;

		constexpr void clear() noexcept;

		constexpr bool get  (const Index index) const noexcept;
		constexpr void set  (const Index index)		  noexcept;
		constexpr void clear(const Index index)		  noexcept;

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <typename, Short>
		friend class BitArray;

	private:
		constexpr explicit CBits(const Unit* const storage,
								 const Index width) noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		constexpr explicit operator bool() const noexcept;

		constexpr bool get(const Index index) const noexcept;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() {
		clear();
	}

	constexpr void clear() noexcept;

	constexpr bool get  (const Index index) const noexcept;
	constexpr void set  (const Index index)		  noexcept;
	constexpr void clear(const Index index)		  noexcept;

	template <Short NUnit, Short NWidth>
	constexpr  Bits bits()		 noexcept;

	template <Short NUnit, Short NWidth>
	constexpr CBits bits() const noexcept;

	constexpr  Bits bits(const Units& units)	   noexcept;
	constexpr CBits bits(const Units& units) const noexcept;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	constexpr void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
