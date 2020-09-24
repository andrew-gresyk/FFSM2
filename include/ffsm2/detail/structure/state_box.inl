namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
DBox<T, TA>::construct() {
	FFSM2_ASSERT(!initialized_);

	new(&t_) T{};

	FFSM2_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
void
DBox<T, TA>::destruct() {
	FFSM2_ASSERT(initialized_);

	t_.~T();

	FFSM2_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
SBox<T, TA>::guard(GuardControlT<TA>& control) {
	t_.widePreEntryGuard(control.context());
	t_.		  entryGuard(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
