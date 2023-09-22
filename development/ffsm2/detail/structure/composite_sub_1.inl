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
TaskStatus
CS_<NN, TA, NI, TL_<TS...>>::widePreUpdate(FullControl& control,
										   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePreUpdate(control, prong) :
		RHalf::widePreUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
TaskStatus
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
TaskStatus
CS_<NN, TA, NI, TL_<TS...>>::widePostUpdate(FullControl& control,
											const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePostUpdate(control, prong) :
		RHalf::widePostUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<TS...>>::widePreReact(FullControl& control,
										  const TEvent& event,
										  const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePreReact(control, event, prong) :
		RHalf::widePreReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
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
TaskStatus
CS_<NN, TA, NI, TL_<TS...>>::widePostReact(FullControl& control,
										   const TEvent& event,
										   const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePostReact(control, event, prong) :
		RHalf::widePostReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<TS...>>::wideQuery(ConstControl& control,
									   TEvent& event,
									   const Short prong) const noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideQuery(control, event, prong) :
		RHalf::wideQuery(control, event, prong);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN, typename TA, Short NI, typename... TS>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<TS...>>::wideUpdatePlans(FullControl& control,
											 const Short prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdatePlans(control, prong) :
		RHalf::wideUpdatePlans(control, prong);
}

#endif

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
