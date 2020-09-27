namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID N, typename TA, Short NI, typename T>
bool
CS_<N, TA, NI, T>::wideEntryGuard(GuardControl& control,
								  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideConstruct(PlanControl& control,
								 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideEnter(PlanControl& control,
							 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideReenter(PlanControl& control,
							   const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
Status
CS_<N, TA, NI, T>::wideUpdate(FullControl& control,
							  const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
template <typename TEvent>
Status
CS_<N, TA, NI, T>::wideReact(FullControl& control,
							 const TEvent& event,
							 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
bool
CS_<N, TA, NI, T>::wideExitGuard(GuardControl& control,
								 const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideExit(PlanControl& control,
							const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, Short NI, typename T>
void
CS_<N, TA, NI, T>::wideDestruct(PlanControl& control,
								const Short FFSM2_IF_ASSERT(prong))
{
	FFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepDestruct(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
