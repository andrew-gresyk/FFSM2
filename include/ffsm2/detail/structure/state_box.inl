namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
DynamicBox<T, TA>::construct() noexcept {
	FFSM2_ASSERT(!initialized_);

	new(&t_) T{};

	FFSM2_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
void
DynamicBox<T, TA>::destruct() noexcept {
	FFSM2_ASSERT(initialized_);

	t_.~T();

	FFSM2_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
StaticBox<T, TA>::guard(GuardControlT<TA>& control) noexcept {
	t_.widePreEntryGuard(control.context());
	t_.		  entryGuard(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
