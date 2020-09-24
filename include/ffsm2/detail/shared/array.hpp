namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

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
	FFSM2_INLINE void clear()														{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	FFSM2_INLINE Long append(const TValue& value);

	template <typename TValue>
	FFSM2_INLINE Long append(TValue&& value);

	template <typename N>
	FFSM2_INLINE	   Item& operator[] (const N i);

	template <typename N>
	FFSM2_INLINE const Item& operator[] (const N i) const;

	FFSM2_INLINE Long count() const													{ return _count;	}

	Array& operator += (const Item& item);
	Array& operator += (Item&& item);

	template <Long N>
	Array& operator += (const Array<Item, N>& other);

	FFSM2_INLINE Iterator<      Array>  begin()			{ return Iterator<		Array>(*this,     0);	}
	FFSM2_INLINE Iterator<const Array>  begin() const	{ return Iterator<const Array>(*this,     0);	}
	FFSM2_INLINE Iterator<const Array> cbegin() const	{ return Iterator<const Array>(*this,     0);	}

	FFSM2_INLINE Iterator<      Array>	  end()			{ return Iterator<		Array>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const Array>	  end() const	{ return Iterator<const Array>(*this, DUMMY);	}
	FFSM2_INLINE Iterator<const Array>   cend() const	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	FFSM2_INLINE Long next(const Long i) const										{ return i + 1;		}
	FFSM2_INLINE Long limit() const													{ return _count;	}

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