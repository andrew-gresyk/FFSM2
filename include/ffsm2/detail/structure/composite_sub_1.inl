namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
bool
CS_<N, TA, NI, TS...>::wideEntryGuard(GuardControl& control,
									  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<N, TA, NI, TS...>::wideEnter(PlanControl& control,
								 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<N, TA, NI, TS...>::wideReenter(PlanControl& control,
								   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
Status
CS_<N, TA, NI, TS...>::wideUpdate(FullControl& control,
								  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
Status
CS_<N, TA, NI, TS...>::wideReact(FullControl& control,
								 const TEvent& event,
								 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
bool
CS_<N, TA, NI, TS...>::wideExitGuard(GuardControl& control,
									 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<N, TA, NI, TS...>::wideExit(PlanControl& control,
								const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID N, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<N, TA, NI, TS...>::wideChangeToRequested(PlanControl& control,
											 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideChangeToRequested(control, prong);
	else
		rHalf.wideChangeToRequested(control, prong);
}

////////////////////////////////////////////////////////////////////////////////

}
}
