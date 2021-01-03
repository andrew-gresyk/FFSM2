namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.active != INVALID_SHORT);

	const Short  requested  = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _subStates.wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepConstruct(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   == INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	active	  = requested;
	requested = INVALID_SHORT;

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (_headState.deepUpdate(control)) {
		ControlLock lock{control};

		_subStates.wideUpdate(control, active);
	} else {
		FFSM2_IF_ANY_PLANS(const Status subStatus =)
		_subStates.wideUpdate(control, active);

	#ifdef FFSM2_ENABLE_ANY_PLANS
		if (subStatus FFSM2_IF_DYNAMIC_PLANS(&& control._planData.planExists))
			control.updatePlan(_headState, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
void
C_<TA, TH, TS...>::deepReact(FullControl& control,
							 const TEvent& event) noexcept
{
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (_headState.deepReact(control, event)) {
		ControlLock lock{control};

		_subStates.wideReact(control, event, active);
	} else {
		FFSM2_IF_ANY_PLANS(const Status subStatus =)
		_subStates.wideReact(control, event, active);

	#ifdef FFSM2_ENABLE_ANY_PLANS
		if (subStatus FFSM2_IF_DYNAMIC_PLANS(&& control._planData.planExists))
			control.updatePlan(_headState, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	const Short  active  = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	return _subStates.wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepDestruct(PlanControl& control) noexcept {
	Short& active = control._registry.active;

	FFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideDestruct(control, active);
	_headState.deepDestruct(control);

	active = INVALID_SHORT;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	auto plan = control.plan();
	plan.clear();
#endif
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
void
C_<TA, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   != INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	if (requested != active) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		active	  = requested;
		requested = INVALID_SHORT;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else {
		requested = INVALID_SHORT;

		// reconstruction done in S_::reenter()
		_subStates.wideReenter	(control, active);
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}
