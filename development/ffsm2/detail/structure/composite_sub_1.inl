namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<TS...>>::wideEntryGuard(GuardControl& control,
											const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideEnter(PlanControl& control,
									   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideReenter(PlanControl& control,
										 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<TS...>>::wideUpdate(FullControl& control,
										const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<TS...>>::wideReverseUpdate(FullControl& control,
											   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideReverseUpdate(control, prong) :
		RHalf::wideReverseUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<TS...>>::wideReact(FullControl& control,
									   const TEvent& event,
									   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
Status
CS_<NN, TA, NI, TL_<TS...>>::wideReverseReact(FullControl& control,
											  const TEvent& event,
											  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideReverseReact(control, event, prong) :
		RHalf::wideReverseReact(control, event, prong);
}

//------------------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<TS...>>::wideExitGuard(GuardControl& control,
										   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideExit(PlanControl& control,
									  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideChangeToRequested(PlanControl& control,
												   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

////////////////////////////////////////////////////////////////////////////////

}
}
