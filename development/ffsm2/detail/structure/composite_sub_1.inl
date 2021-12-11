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
		return LMaterial::wideEntryGuard(control, prong);
	else
		return RMaterial::wideEntryGuard(control, prong);
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
		LMaterial::wideEnter(control, prong);
	else
		RMaterial::wideEnter(control, prong);
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
		LMaterial::wideReenter(control, prong);
	else
		RMaterial::wideReenter(control, prong);
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
		LMaterial::wideUpdate(control, prong) :
		RMaterial::wideUpdate(control, prong);
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
		LMaterial::wideReact(control, event, prong) :
		RMaterial::wideReact(control, event, prong);
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
		return LMaterial::wideExitGuard(control, prong);
	else
		return RMaterial::wideExitGuard(control, prong);
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
		LMaterial::wideExit(control, prong);
	else
		RMaterial::wideExit(control, prong);
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
		LMaterial::wideChangeToRequested(control, prong);
	else
		RMaterial::wideChangeToRequested(control, prong);
}

////////////////////////////////////////////////////////////////////////////////

}
}
