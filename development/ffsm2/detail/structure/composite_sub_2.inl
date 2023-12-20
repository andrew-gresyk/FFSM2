namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<T>>::wideEntryGuard(GuardControl& control,
										   const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideEnter(PlanControl& control,
									  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideReenter(PlanControl& control,
										const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePreUpdate(FullControl& control,
										  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::wideUpdate(FullControl& control,
									   const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePostUpdate(FullControl& control,
										   const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePreReact(FullControl& control,
										 const TEvent& event,
										 const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::wideReact(FullControl& control,
									  const TEvent& event,
									  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::widePostReact(FullControl& control,
										  const TEvent& event,
										  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostReact(control, event);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename T>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideQuery(ConstControl& control,
									  TEvent& event,
									  const Prong FFSM2_IF_ASSERT(prong)) const noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
TaskStatus
CS_<NN_, TA_, NP_, TL_<T>>::wideUpdatePlans(FullControl& control,
											const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdatePlans(control);
}

#endif

//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
bool
CS_<NN_, TA_, NP_, TL_<T>>::wideExitGuard(GuardControl& control,
										  const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_, Prong NP_, typename T>
FFSM2_CONSTEXPR(14)
void
CS_<NN_, TA_, NP_, TL_<T>>::wideExit(PlanControl& control,
									 const Prong FFSM2_IF_ASSERT(prong)) noexcept
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepExit(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
