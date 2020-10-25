namespace ffsm2 {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
	template <typename TArgs>
	static constexpr void execute(ffsm2::detail::GuardControlT<TArgs>&) noexcept {}
};

namespace detail {

//------------------------------------------------------------------------------

FFSM2_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DynamicBox final {
	using Type = T;

	static constexpr bool isBare() noexcept							{ return false;						}

	union {
		Type t_;
	};

	constexpr DynamicBox() noexcept {}
	~DynamicBox()		   noexcept {}

	constexpr void guard(GuardControlT<TArgs>& control) noexcept	{ Guard<Type>::execute(control);	}

	constexpr void construct() noexcept;
	constexpr void destruct()  noexcept;

	constexpr		Type& get()		  noexcept				{ FFSM2_ASSERT(initialized_); return t_;	}
	constexpr const Type& get() const noexcept				{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare() noexcept	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	constexpr void guard(GuardControlT<TArgs>& control) noexcept;

	constexpr void construct() noexcept 																{}
	constexpr void destruct()  noexcept 																{}

	constexpr		Type& get()		  noexcept						{ return t_;						}
	constexpr const Type& get() const noexcept						{ return t_;						}

	FFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = typename std::conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DynamicBox<T, TArgs>,
					 StaticBox <T, TArgs>
				 >::type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}
}

#include "state_box.inl"
