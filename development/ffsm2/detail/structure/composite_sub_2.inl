namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<T>>::wideEntryGuard(GuardControl& control,
										const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideEnter(PlanControl& control,
								   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideReenter(PlanControl& control,
									 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::widePreUpdate(FullControl& control,
									   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::wideUpdate(FullControl& control,
									const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::widePostUpdate(FullControl& control,
										const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::widePreReact(FullControl& control,
									  const TEvent& event,
									  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::wideReact(FullControl& control,
								   const TEvent& event,
								   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, Short NI, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::widePostReact(FullControl& control,
									   const TEvent& event,
									   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostReact(control, event);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideQuery(ConstControl& control,
								   TEvent& event,
								   const Short FFSM2_IF_ASSERT(prong)) const noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN, TA, NI, TL_<T>>::wideUpdatePlans(FullControl& control,
										 const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdatePlans(control);
}

#endif

//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN, TA, NI, TL_<T>>::wideExitGuard(GuardControl& control,
									   const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, Short NI, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN, TA, NI, TL_<T>>::wideExit(PlanControl& control,
								  const Short FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepExit(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
