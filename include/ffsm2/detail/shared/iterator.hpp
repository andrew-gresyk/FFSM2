namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;

	template <typename T, Long NCapacity>
	friend class Array;

private:
	FFSM2_INLINE Iterator(Container& container,
						  const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const Iterator<Container>& dummy) const noexcept;

	FFSM2_INLINE Iterator& operator ++() noexcept;

	FFSM2_INLINE	   Item& operator *()		noexcept	{ return  _container[_cursor];	}
	FFSM2_INLINE const Item& operator *() const noexcept	{ return  _container[_cursor];	}

	FFSM2_INLINE	   Item* operator->()		noexcept	{ return &_container[_cursor];	}
	FFSM2_INLINE const Item* operator->() const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Long _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, Long NCapacity>
	friend class Array;

private:
	FFSM2_INLINE Iterator(const Container& container,
						  const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	FFSM2_INLINE bool operator != (const Iterator<const Container>& dummy) const noexcept;

	FFSM2_INLINE Iterator& operator ++() noexcept;

	FFSM2_INLINE const Item& operator *() const noexcept	{ return _container[_cursor];	}

	FFSM2_INLINE const Item* operator->() const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "iterator.inl"
