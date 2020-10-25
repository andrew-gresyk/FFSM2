namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class StaticArray {
public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item  = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	constexpr StaticArray()									noexcept = default;
	constexpr StaticArray(const Item filler)				noexcept;

	template <typename N>
	constexpr		Item& operator[] (const N i)			noexcept;

	template <typename N>
	constexpr const Item& operator[] (const N i) const		noexcept;

	constexpr Long count() const							noexcept	{ return CAPACITY;									}

	constexpr void fill(const Item filler)					noexcept;
	constexpr void clear()									noexcept	{ fill(INVALID_SHORT);								}

	constexpr Iterator<      StaticArray>  begin()			noexcept	{ return Iterator<      StaticArray>(*this,     0); }
	constexpr Iterator<const StaticArray>  begin() const	noexcept	{ return Iterator<const StaticArray>(*this,     0); }
	constexpr Iterator<const StaticArray> cbegin() const	noexcept	{ return Iterator<const StaticArray>(*this,     0); }

	constexpr Iterator<      StaticArray>    end()			noexcept	{ return Iterator<      StaticArray>(*this, DUMMY);	}
	constexpr Iterator<const StaticArray>    end() const	noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}
	constexpr Iterator<const StaticArray>   cend() const	noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	constexpr StaticArray() = default;
	constexpr StaticArray(const Item)											{}
};

//------------------------------------------------------------------------------

template <typename T, Long NCapacity>
class Array {
	template <typename>
	friend class Iterator;

public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	constexpr void clear()										noexcept			{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	constexpr Long append(const TValue& value)					noexcept;

	template <typename TValue>
	constexpr Long append(	   TValue&& value)					noexcept;

	template <typename N>
	constexpr		Item& operator[] (const N i)				noexcept;

	template <typename N>
	constexpr const Item& operator[] (const N i) const			noexcept;

	constexpr Long count() const								noexcept	{ return _count;	}

	constexpr Array& operator += (const Item& item)				noexcept;
	constexpr Array& operator += (	   Item&& item)				noexcept;

	template <Long N>
	constexpr Array& operator += (const Array<Item, N>& other)	noexcept;

	constexpr Iterator<      Array>  begin()					noexcept	{ return Iterator<		Array>(*this,     0);	}
	constexpr Iterator<const Array>  begin() const				noexcept	{ return Iterator<const Array>(*this,     0);	}
	constexpr Iterator<const Array> cbegin() const				noexcept	{ return Iterator<const Array>(*this,     0);	}

	constexpr Iterator<      Array>	  end()						noexcept	{ return Iterator<		Array>(*this, DUMMY);	}
	constexpr Iterator<const Array>	  end()  const				noexcept	{ return Iterator<const Array>(*this, DUMMY);	}
	constexpr Iterator<const Array>   cend() const				noexcept	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	constexpr Long next(const Long i) const						noexcept	{ return i + 1;		}
	constexpr Long limit() const								noexcept	{ return _count;	}

private:
	Long _count = 0;

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
class Array<T, 0> {
public:
	static constexpr Long CAPACITY = 0;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<0>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
