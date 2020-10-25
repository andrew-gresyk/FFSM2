namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID N, typename TA, typename TH>
constexpr bool
S_<N, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.guard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::deepConstruct(PlanControl& FFSM2_IF_LOG_INTERFACE(control)) noexcept {
	FFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::deepEnter(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::deepReenter(PlanControl& control) noexcept {
	FFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.destruct ();
	_headBox.construct();

	_headBox.get().widePreReenter(control.context());
	_headBox.get().		  reenter(control);
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr Status
S_<N, TA, TH>::deepUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreUpdate(control.context());
	_headBox.get().		  update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
template <typename TEvent>
constexpr Status
S_<N, TA, TH>::deepReact(FullControl& control,
						 const TEvent& event) noexcept
{
	auto reaction = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	FFSM2_LOG_STATE_METHOD(reaction,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreReact(event, control.context());
	(_headBox.get().*reaction) (event, control);				//_headBox.get().react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr bool
S_<N, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.get().widePreExitGuard(control.context());
	_headBox.get().		  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::deepExit(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from ffsm2::Machine::Instance::State
	_headBox.get().		   exit(control);
	_headBox.get().widePostExit(control.context());
}

//------------------------------------------------------------------------------

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::deepDestruct(PlanControl&
						#if defined FFSM2_ENABLE_LOG_INTERFACE || defined FFSM2_ENABLE_PLANS
							control
						#endif
							) noexcept
{
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::DESTRUCT);

	_headBox.destruct();

#ifdef FFSM2_ENABLE_PLANS
	control._planData.clearTaskStatus(STATE_ID);
#endif
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID N, typename TA, typename TH>
constexpr void
S_<N, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planFailed(control);
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
