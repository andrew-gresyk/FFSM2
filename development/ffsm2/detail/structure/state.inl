namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
bool
S_<NN, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideEntryGuard(control);
	Head::	  entryGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepEnter(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideEnter(control);
	Head::	  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepReenter(PlanControl& control) noexcept {
	FFSM2_IF_PLANS(control._core.planData.verifyEmptyStatus(STATE_ID));

	FFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReenter(control);
	Head::	  reenter(control);
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepPreUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::preUpdate,
						   Method::PRE_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreUpdate(control);
	Head::	  preUpdate(control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideUpdate(control);
	Head::	  update(control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepPostUpdate(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::postUpdate,
						   Method::POST_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::	  postUpdate(control);
	Head::widePostUpdate(control);

	return control._taskStatus;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepPreReact(FullControl& control,
							 const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::preReact);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::PRE_REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReact(event, control);
	(this->*method)	  (event, control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepReact(FullControl& control,
						  const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReact(event, control);
	(this->*method)(event, control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepPostReact(FullControl& control,
							  const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::postReact);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::POST_REACT);

	ScopedOrigin origin{control, STATE_ID};

	(this->*method)	   (event, control);
	Head::widePostReact(event, control);

	return control._taskStatus;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepQuery(ConstControl& control,
						  TEvent&  event) const noexcept
{
	auto method = static_cast<void (Head::*)(TEvent&, ConstControl&) const>(&Head::query);

	FFSM2_LOG_STATE_METHOD(method,
						   Method::QUERY);

	ScopedCOrigin origin{control, STATE_ID};

	(this->*method)(event, control);
	Head::wideQuery(event, control);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN, TA, TH>::deepUpdatePlans(FullControl& control) noexcept {
	if (control._core.planData.tasksFailures .get(STATE_ID))
		return TaskStatus{TaskStatus::FAILURE};
	else
	if (control._core.planData.tasksSuccesses.get(STATE_ID))
		return TaskStatus{TaskStatus::SUCCESS};
	else
		return TaskStatus{};
}

#endif

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
bool
S_<NN, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideExitGuard(control);
	Head::	  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::deepExit(PlanControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from ffsm2::Machine::Instance::State
	Head::	  exit(control);
	Head::wideExit(control);

	FFSM2_IF_PLANS(control._core.planData.clearTaskStatus(STATE_ID));
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN, typename TA, typename TH>
FFSM2_CONSTEXPR(14)
void
S_<NN, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	FFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planFailed(control);
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
