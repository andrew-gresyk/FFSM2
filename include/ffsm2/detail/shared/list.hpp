namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, Long NCapacity>
class List {
public:
	using Index = Long;

	static constexpr Index CAPACITY = NCapacity;

	static constexpr Index INVALID = Index (-1);

private:
	using Item  = TItem;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Links {
		Index prev;
		Index next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	union Cell {
		Item item;
		Links links;

		constexpr Cell() noexcept
			: links{}
		{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	template <typename... TArgs>
	constexpr Index emplace(TArgs... args) noexcept;

	constexpr void remove(const Index i) noexcept;

	constexpr		Item& operator[] (const Index i)	   noexcept;
	constexpr const Item& operator[] (const Index i) const noexcept;

	constexpr Index count() const noexcept						{ return _count;	}

private:
	FFSM2_IF_ASSERT(constexpr void verifyStructure(const Index occupied = INVALID) const noexcept);

private:
	Cell _cells[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last = 0;
	Index _count = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class List<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "list.inl"
