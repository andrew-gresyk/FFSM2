namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(11)
R_<TG_, TA_>::R_(Context& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{context
		  FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(11)
R_<TG_, TA_>::R_(PureContext&& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{move(context)
		  FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(20)
R_<TG_, TA_>::~R_() noexcept {
	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::update() noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	Transition emptyTransition;
	FullControl control{_core, emptyTransition};

	_apex. deepPreUpdate(control);
	_apex.	  deepUpdate(control);
	_apex.deepPostUpdate(control);

#if FFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearRegionStatuses();
#endif

	processRequest();
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::react(const TEvent& event) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	Transition emptyTransition;
	FullControl control{_core, emptyTransition};

	_apex. deepPreReact(control, event);
	_apex.	  deepReact(control, event);
	_apex.deepPostReact(control, event);

#if FFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearRegionStatuses();
#endif

	processRequest();
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::query(TEvent& event) const noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	ConstControl control{_core};

	_apex.deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::succeed(const StateID stateId_) noexcept {
	_core.planData.tasksSuccesses.set(stateId_);

	FFSM2_LOG_TASK_STATUS(_core.context, stateId_, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::fail(const StateID stateId_) noexcept {
	_core.planData.tasksFailures.set(stateId_);

	FFSM2_LOG_TASK_STATUS(_core.context, stateId_, StatusEvent::FAILED);
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::changeTo(const StateID stateId_) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request = Transition{stateId_};

	FFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, stateId_);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateChangeTo(const StateID stateId_) noexcept {
	changeTo(stateId_);

	processRequest();
}

//------------------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::replayTransition(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.previousTransition.clear();

	if (FFSM2_CHECKED(destination != INVALID_SHORT)) {
		Transition currentTransition;
		PlanControl control{_core, currentTransition};

		applyRequest(currentTransition,
					 destination);

		_core.previousTransition = Transition{destination};

		_apex.deepChangeToRequested(control);

		_core.registry.clearRequests();

		FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));

		return true;
	}

	return false;
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::initialEnter() noexcept {
	FFSM2_ASSERT(!_core.registry.isActive());
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	Transition currentTransition;
	Transition pendingTransition;

	PlanControl control{_core, currentTransition};
	applyRequest(currentTransition, 0);

	cancelledByEntryGuards(currentTransition,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _core.request;
		 ++i)
	{
		//backup();

		if (applyRequest(currentTransition,
						 _core.request.destination))
		{
			pendingTransition = _core.request;
			_core.request.clear();

			if (cancelledByEntryGuards(currentTransition,
									   pendingTransition))
				FFSM2_BREAK();
			else
				currentTransition = pendingTransition;

			pendingTransition.clear();
		} else
			_core.request.clear();
	}
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = currentTransition);

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::finalExit() noexcept {
	FFSM2_ASSERT(_core.registry.isActive());
	FFSM2_ASSERT(!_core.request);

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepExit(control);

	_core.registry.clear();
	_core.request .clear();

#if FFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.previousTransition.clear();
#endif
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::processRequest() noexcept {
	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));

	Transition currentTransition;

	if (_core.request) {
		processTransitions(currentTransition);

		FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
	}

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = currentTransition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::processTransitions(Transition& currentTransition) noexcept {
	FFSM2_ASSERT(_core.request);

	PlanControl control{_core, currentTransition};

	Transition pendingTransition;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _core.request;
		++i)
	{
		//backup();

		if (applyRequest(currentTransition,
						 _core.request.destination))
		{
			pendingTransition = _core.request;
			_core.request.clear();

			if (cancelledByGuards(currentTransition,
								  pendingTransition))
				;
			else
				currentTransition = pendingTransition;

			pendingTransition.clear();
		} else
			_core.request.clear();
	}
	FFSM2_ASSERT(!_core.request);

	if (currentTransition)
		_apex.deepChangeToRequested(control);

	_core.registry.clearRequests();
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::applyRequest(const Transition& currentTransition,
						   const StateID destination) noexcept
{
	if (currentTransition != Transition{destination}) {
		_core.registry.requested = destination;

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::cancelledByEntryGuards(const Transition& currentTransition,
									 const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_core
							, currentTransition
							, pendingTransition};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::cancelledByGuards(const Transition& currentTransition,
								const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_core
							, currentTransition
							, pendingTransition};

	return _apex.deepForwardExitGuard (guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::save(WriteStream& stream) const noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_apex.deepSaveActive(_core.registry, stream);

	// TODO: save(stream, _core.requests);

#if FFSM2_PLANS_AVAILABLE()
	// TODO: save(stream, _core.planData);
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: save(stream, _core.rng);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	// TODO: save(stream, _core.transitionTarget);
	// TODO: save(stream, _core.previousTransition);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
FFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::load(ReadStream& stream) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.registry.clearRequests();
	_apex.deepLoadRequested(_core.registry, stream);

	_core.request.clear();
	// TODO: load(stream, _core.requests);

#if FFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
	// TODO: load(stream, _core.planData);
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: load(stream, _core.rng);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.previousTransition.clear();
#endif

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepChangeToRequested(control);
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

////////////////////////////////////////////////////////////////////////////////

}
}
