namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA, typename TH>
bool
S_<TN_, TA, TH>::deepEntryGuard(GuardControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   control.context(),
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.guard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepConstruct(PlanControl& FFSM2_IF_LOG_INTERFACE(control)) {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   control.context(),
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepEnter(PlanControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   control.context(),
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepReenter(PlanControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::reenter,
						   control.context(),
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.destruct ();
	_headBox.construct();

	_headBox.get().widePreReenter(control.context());
	_headBox.get().		  reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
Status
S_<TN_, TA, TH>::deepUpdate(FullControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::update,
						   control.context(),
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreUpdate(control.context());
	_headBox.get().		  update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
Status
S_<TN_, TA, TH>::deepReact(FullControl& control,
						   const TEvent& event)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	FFSM2_LOG_STATE_METHOD(reaction,
						   control.context(),
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreReact(event, control.context());
	(_headBox.get().*reaction) (event, control);				//_headBox.get().react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
bool
S_<TN_, TA, TH>::deepExitGuard(GuardControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   control.context(),
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.get().widePreExitGuard(control.context());
	_headBox.get().		  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepExit(PlanControl& control) {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   control.context(),
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

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepDestruct(PlanControl&
						  #if defined FFSM2_ENABLE_LOG_INTERFACE || defined FFSM2_ENABLE_PLANS
							  control
						  #endif
							  )
{
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   control.context(),
						   Method::DESTRUCT);

	_headBox.destruct();
}

///////////////////////////////////////////////////////////////////////////////

}
}
