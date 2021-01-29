namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, unsigned>
	friend class ArrayT;

private:
	FFSM2_INLINE IteratorT(Container& container,
						   const Index cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const IteratorT<Container>& other) const noexcept;

	FFSM2_INLINE IteratorT& operator ++()		  noexcept;

	FFSM2_INLINE	   Item& operator *()		  noexcept	{ return  _container[_cursor];	}
	FFSM2_INLINE const Item& operator *()	const noexcept	{ return  _container[_cursor];	}

	FFSM2_INLINE	   Item* operator->()		  noexcept	{ return &_container[_cursor];	}
	FFSM2_INLINE const Item* operator->()	const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Long _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, unsigned>
	friend class ArrayT;

private:
	FFSM2_INLINE IteratorT(const Container& container,
						   const Index cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const IteratorT<const Container>& other) const noexcept;

	FFSM2_INLINE IteratorT& operator ++()		  noexcept;

	FFSM2_INLINE const Item& operator *()	const noexcept	{ return _container[_cursor];	}

	FFSM2_INLINE const Item* operator->()	const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "iterator.inl"
