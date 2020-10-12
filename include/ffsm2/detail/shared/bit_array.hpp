namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	inline Units(Short unit_  = INVALID_SHORT,
				 Short width_ = INVALID_SHORT)
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
		FFSM2_INLINE explicit Bits(Unit* const storage,
								   const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		FFSM2_INLINE explicit operator bool() const;

		FFSM2_INLINE void clear();

		FFSM2_INLINE bool get  (const Index index) const;
		FFSM2_INLINE void set  (const Index index);
		FFSM2_INLINE void clear(const Index index);

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class ConstBits {
		template <typename, Short>
		friend class BitArray;

	private:
		FFSM2_INLINE explicit ConstBits(const Unit* const storage,
										const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		FFSM2_INLINE explicit operator bool() const;

		FFSM2_INLINE bool get(const Index index) const;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() {
		clear();
	}

	FFSM2_INLINE void clear();

	FFSM2_INLINE bool get  (const Index index) const;
	FFSM2_INLINE void set  (const Index index);
	FFSM2_INLINE void clear(const Index index);

	template <Short NUnit, Short NWidth>
	FFSM2_INLINE	  Bits bits();

	template <Short NUnit, Short NWidth>
	FFSM2_INLINE ConstBits bits() const;


	FFSM2_INLINE	  Bits bits(const Units& units);
	FFSM2_INLINE ConstBits bits(const Units& units) const;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	FFSM2_INLINE void clear() {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
