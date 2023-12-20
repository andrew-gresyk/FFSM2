namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideEntryGuard(GuardControl& control,
												const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideEnter(PlanControl& control,
										   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideReenter(PlanControl& control,
											 const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePreUpdate(FullControl& control,
											   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePreUpdate(control, prong) :
		RHalf::widePreUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideUpdate(FullControl& control,
											const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePostUpdate(FullControl& control,
												const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePostUpdate(control, prong) :
		RHalf::widePostUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePreReact(FullControl& control,
											  const TEvent& event,
											  const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePreReact(control, event, prong) :
		RHalf::widePreReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideReact(FullControl& control,
										   const TEvent& event,
										   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::widePostReact(FullControl& control,
											   const TEvent& event,
											   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePostReact(control, event, prong) :
		RHalf::widePostReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideQuery(ConstControl& control,
										   TEvent& event,
										   const Prong prong) const noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideQuery(control, event, prong) :
		RHalf::wideQuery(control, event, prong);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideUpdatePlans(FullControl& control,
												 const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideUpdatePlans(control, prong) :
		RHalf::wideUpdatePlans(control, prong);
}

#endif

//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideExitGuard(GuardControl& control,
											   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideExit(PlanControl& control,
										  const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

//------------------------------------------------------------------------------
// COMMON

template <StateID NN_, typename TA_, Prong NP_, typename... TS_>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<TS_...>>::wideChangeToRequested(PlanControl& control,
													   const Prong prong) noexcept
{
	FFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

////////////////////////////////////////////////////////////////////////////////

}
}
