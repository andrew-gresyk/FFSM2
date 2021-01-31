namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned NCapacity>
class StaticArrayT {
public:
	using Item  = T;
	using Index = UnsignedCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index DUMMY	= (Index) -1;

public:
	FFSM2_INLINE StaticArrayT() = default;
	FFSM2_INLINE StaticArrayT(const Item filler)					  noexcept;

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i)					  noexcept;

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i)				const noexcept;

	FFSM2_INLINE Index count()									const noexcept	{ return CAPACITY;										}

	FFSM2_INLINE void fill(const Item filler)						  noexcept;
	FFSM2_INLINE void clear()										  noexcept	{ fill(INVALID_SHORT);									}

	FFSM2_INLINE IteratorT<      StaticArrayT>  begin()				  noexcept	{ return IteratorT<      StaticArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const StaticArrayT>  begin()			const noexcept	{ return IteratorT<const StaticArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const StaticArrayT> cbegin()			const noexcept	{ return IteratorT<const StaticArrayT>(*this,     0);	}

	FFSM2_INLINE IteratorT<      StaticArrayT>    end()				  noexcept	{ return IteratorT<      StaticArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const StaticArrayT>    end()			const noexcept	{ return IteratorT<const StaticArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const StaticArrayT>   cend()			const noexcept	{ return IteratorT<const StaticArrayT>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArrayT<T, 0> {
	using Item  = T;

	FFSM2_INLINE StaticArrayT() = default;
	FFSM2_INLINE StaticArrayT(const Item) noexcept {}
};

//------------------------------------------------------------------------------

template <typename T, unsigned NCapacity>
class ArrayT {
	template <typename>
	friend class IteratorT;

public:
	using Item	= T;
	using Index = UnsignedCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index DUMMY	= (Index) -1;

public:
	FFSM2_INLINE void clear()										  noexcept	{ _count = 0;									}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	FFSM2_INLINE Index append(const TValue&  value)					  noexcept;

	template <typename TValue>
	FFSM2_INLINE Index append(		TValue&& value)					  noexcept;

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i)					  noexcept;

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i)				const noexcept;

	FFSM2_INLINE Index count()									const noexcept	{ return _count;	}

	FFSM2_INLINE ArrayT& operator += (const Item& item)				  noexcept;
	FFSM2_INLINE ArrayT& operator += (	  Item&& item)				  noexcept;

	template <Long N>
	FFSM2_INLINE ArrayT& operator += (const ArrayT<Item, N>& other)	  noexcept;

	FFSM2_INLINE IteratorT<      ArrayT>  begin()					  noexcept	{ return IteratorT<		 ArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const ArrayT>  begin()				const noexcept	{ return IteratorT<const ArrayT>(*this,     0);	}
	FFSM2_INLINE IteratorT<const ArrayT> cbegin()				const noexcept	{ return IteratorT<const ArrayT>(*this,     0);	}

	FFSM2_INLINE IteratorT<      ArrayT>	end()					  noexcept	{ return IteratorT<		 ArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const ArrayT>	end()				const noexcept	{ return IteratorT<const ArrayT>(*this, DUMMY);	}
	FFSM2_INLINE IteratorT<const ArrayT>   cend()				const noexcept	{ return IteratorT<const ArrayT>(*this, DUMMY);	}

private:
	FFSM2_INLINE Index next(const Index i)						const noexcept	{ return i + 1;		}
	FFSM2_INLINE Index limit()									const noexcept	{ return _count;	}

private:
	Index _count = 0;

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	Item _items[CAPACITY]; // warning 4324 triggers for 'StructureStateInfo'

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class ArrayT<T, 0> {
public:
	using Item	= T;
	using Index = UnsignedCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
