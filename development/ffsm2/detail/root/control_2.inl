namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanControlT<TArgs>::Region::Region(PlanControlT& control_) noexcept
	: control  {control_}
{
	control.setRegion();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::setRegion() noexcept
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::resetRegion() noexcept
{
	_taskStatus.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
