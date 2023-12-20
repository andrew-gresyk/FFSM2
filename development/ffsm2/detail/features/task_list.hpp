#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload, Long NCapacity>
class TaskListT {
public:
	using Index		= Long;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index INVALID	= Index (-1);

private:
	using Payload	= TPayload;
	using Item		= TaskT<Payload>;

public:
	FFSM2_CONSTEXPR(14)	 void clear()											noexcept;

	template <typename... TArgs>
	FFSM2_CONSTEXPR(14)	Index emplace(TArgs&&... args)							noexcept;

	FFSM2_CONSTEXPR(14)	void remove(const Index i)								noexcept;

	FFSM2_CONSTEXPR(14)		  Item& operator[] (const Index i)					noexcept;
	FFSM2_CONSTEXPR(11)	const Item& operator[] (const Index i)			  const noexcept;

	FFSM2_CONSTEXPR(11)	Index count()									  const noexcept	{ return _count;		}
	FFSM2_CONSTEXPR(11)	bool  empty()									  const noexcept	{ return _count == 0;	}

private:
	FFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)  const noexcept);

private:
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last		  = 0;
	Index _count	  = 0;
	Item _items[CAPACITY] {};
};

//------------------------------------------------------------------------------

template <typename TItem>
class TaskListT<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "task_list.inl"

#endif
