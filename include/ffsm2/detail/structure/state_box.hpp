namespace ffsm2 {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
	template <typename TArgs>
	static void execute(ffsm2::detail::GuardControlT<TArgs>&) {}
};

namespace detail {

//------------------------------------------------------------------------------

FFSM2_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DynamicBox final {
	using Type = T;

	static constexpr bool isBare()							{ return false;						}

	union {
		Type t_;
	};

	FFSM2_INLINE  DynamicBox() {}
	FFSM2_INLINE ~DynamicBox() {}

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control)	{ Guard<Type>::execute(control);	}

	FFSM2_INLINE void construct();
	FFSM2_INLINE void destruct();

	FFSM2_INLINE	   Type& get()					{ FFSM2_ASSERT(initialized_); return t_;	}
	FFSM2_INLINE const Type& get() const			{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare()	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control);

	FFSM2_INLINE void construct()																{}
	FFSM2_INLINE void destruct()																{}

	FFSM2_INLINE	   Type& get()							{ return t_;						}
	FFSM2_INLINE const Type& get() const					{ return t_;						}

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
