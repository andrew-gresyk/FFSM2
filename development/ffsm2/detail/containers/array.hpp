namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_CONSTEXPR(11)
T
filler()																noexcept	{
	return T{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
FFSM2_CONSTEXPR(11)
Short
filler<Short>()															noexcept	{
	return INVALID_SHORT;
}

//------------------------------------------------------------------------------

template <typename T, Long NCapacity>
class StaticArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      StaticArrayT>;
	using CIterator	= IteratorT<const StaticArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	FFSM2_CONSTEXPR(14)	StaticArrayT() = default;
	FFSM2_CONSTEXPR(14)	StaticArrayT(const Item filler)					noexcept	{ fill(filler);						}

	template <typename N>
	FFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)			noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)	  const noexcept;

	FFSM2_CONSTEXPR(11)	Index count()							  const noexcept	{ return CAPACITY;					}

	FFSM2_CONSTEXPR(14)	void fill(const Item filler)					noexcept;
	FFSM2_CONSTEXPR(14)	void clear()									noexcept	{ fill(filler<Item>());				}
	FFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	FFSM2_CONSTEXPR(14)	 Iterator  begin()								noexcept	{ return  Iterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator  begin()						  const noexcept	{ return CIterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator cbegin()						  const noexcept	{ return CIterator(*this, first());	}

	FFSM2_CONSTEXPR(14)	 Iterator	 end()								noexcept	{ return  Iterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	 end()						  const noexcept	{ return CIterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	cend()						  const noexcept	{ return CIterator(*this, limit());	}

private:
	FFSM2_CONSTEXPR(11)	Index first()							  const noexcept	{ return 0;							}
	FFSM2_CONSTEXPR(11)	Index  next(const Index index)			  const noexcept	{ return index + 1;					}
	FFSM2_CONSTEXPR(11)	Index limit()							  const noexcept	{ return CAPACITY;					}

private:
	Item _items[CAPACITY] {};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArrayT<T, 0> final {
	using Item		= T;

	FFSM2_CONSTEXPR(11)	StaticArrayT() = default;
	FFSM2_CONSTEXPR(11)	StaticArrayT(const Item)						noexcept	{}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class DynamicArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      DynamicArrayT>;
	using CIterator	= IteratorT<const DynamicArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(const TArgs &... args)				noexcept;

	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(		TArgs&&... args)				noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				noexcept;

	template <typename N>
	FFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)		  const noexcept;

	FFSM2_CONSTEXPR(11)	Index  count()								  const noexcept	{ return _count;					}

	FFSM2_CONSTEXPR(14)	 void clear()										noexcept	{ _count = 0;						}
	FFSM2_CONSTEXPR(11)	 bool empty()								  const noexcept	{ return _count == 0;				}

	// SPECIFIC
	FFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (const Item & item)		noexcept;
	FFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (	  Item&& item)		noexcept;
	// SPECIFIC

	template <Long N>
	FFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (const DynamicArrayT<Item, N>& other)	noexcept;

	FFSM2_CONSTEXPR(14)	 Iterator  begin()									noexcept	{ return  Iterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator  begin()							  const noexcept	{ return CIterator(*this, first());	}
	FFSM2_CONSTEXPR(11)	CIterator cbegin()							  const noexcept	{ return CIterator(*this, first());	}

	FFSM2_CONSTEXPR(14)	 Iterator	 end()									noexcept	{ return  Iterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	 end()							  const noexcept	{ return CIterator(*this, limit());	}
	FFSM2_CONSTEXPR(11)	CIterator	cend()							  const noexcept	{ return CIterator(*this, limit());	}

private:
	FFSM2_CONSTEXPR(11)	Index first()								  const noexcept	{ return 0;							}
	FFSM2_CONSTEXPR(11)	Index next(const Index index)				  const noexcept	{ return index + 1;					}
	FFSM2_CONSTEXPR(11)	Index limit()								  const noexcept	{ return _count;					}

private:
	Index _count = 0;
	Item _items[CAPACITY] {};
};

//------------------------------------------------------------------------------

template <typename T>
class DynamicArrayT<T, 0> final {
public:
	using Item	= T;
	using Index	= UCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
