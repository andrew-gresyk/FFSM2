namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Short NI, typename T>
bool
CS_<TN, TA, NI, T>::wideEntryGuard(GuardControl& control,
								   const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
void
CS_<TN, TA, NI, T>::wideConstruct(PlanControl& control,
								  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
void
CS_<TN, TA, NI, T>::wideEnter(PlanControl& control,
							  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename T>
void
CS_<TN, TA, NI, T>::wideReenter(PlanControl& control,
								const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
Status
CS_<TN, TA, NI, T>::wideUpdate(FullControl& control,
							   const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
template <typename TEvent>
Status
CS_<TN, TA, NI, T>::wideReact(FullControl& control,
							  const TEvent& event,
							  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
bool
CS_<TN, TA, NI, T>::wideExitGuard(GuardControl& control,
								  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
void
CS_<TN, TA, NI, T>::wideExit(PlanControl& control,
							 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename T>
void
CS_<TN, TA, NI, T>::wideDestruct(PlanControl& control,
								 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepDestruct(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
