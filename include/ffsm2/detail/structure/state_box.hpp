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
struct DBox final {
	static constexpr bool isBare()								{ return false;					}

	union {
		T t_;
	};

	FFSM2_INLINE  DBox() {}
	FFSM2_INLINE ~DBox() {}

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control)		{ Guard<T>::execute(control);	}

	FFSM2_INLINE void construct();
	FFSM2_INLINE void destruct();

	FFSM2_INLINE	   T& get()						{ FFSM2_ASSERT(initialized_); return t_;	}
	FFSM2_INLINE const T& get() const				{ FFSM2_ASSERT(initialized_); return t_;	}

	FFSM2_IF_ASSERT(bool initialized_ = false);

	FFSM2_IF_DEBUG(const std::type_index TYPE = typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct SBox final {
	static constexpr bool isBare()	{ return std::is_base_of<T, StaticEmptyT<TArgs>>::value;	}

	T t_;

	FFSM2_INLINE void guard(GuardControlT<TArgs>& control);

	FFSM2_INLINE void construct()																{}
	FFSM2_INLINE void destruct()																{}

	FFSM2_INLINE	   T& get()									{ return t_;					}
	FFSM2_INLINE const T& get() const							{ return t_;					}

	FFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = typename std::conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DBox<T, TArgs>,
					 SBox<T, TArgs>
				 >::type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}
}

#include "state_box.inl"
