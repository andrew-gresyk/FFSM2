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
	constexpr Iterator(Container& container,
					   const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	constexpr bool operator != (const Iterator<Container>& dummy) const noexcept;

	constexpr Iterator& operator ++() noexcept;

	constexpr		Item& operator *()		 noexcept	{ return  _container[_cursor];	}
	constexpr const Item& operator *() const noexcept	{ return  _container[_cursor];	}

	constexpr		Item* operator->()		 noexcept	{ return &_container[_cursor];	}
	constexpr const Item* operator->() const noexcept	{ return &_container[_cursor];	}

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
	constexpr Iterator(const Container& container,
					   const Long cursor) noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	constexpr bool operator != (const Iterator<const Container>& dummy) const noexcept;

	constexpr Iterator& operator ++() noexcept;

	constexpr const Item& operator *() const noexcept	{ return _container[_cursor];	}

	constexpr const Item* operator->() const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "iterator.inl"
