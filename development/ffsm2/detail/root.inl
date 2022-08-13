namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(Context& context
			 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{context
		  FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(11)
R_<TG, TA>::R_(PureContext&& context
			 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{move(context)
		  FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(20)
R_<TG, TA>::~R_() noexcept {
	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::update() noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	FullControl control{_core};

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

template <typename TG, typename TA>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::react(const TEvent& event) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	FullControl control{_core};

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

template <typename TG, typename TA>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::query(TEvent& event) const noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	ConstControl control{_core};

	_apex.deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::succeed(const StateID stateId_) noexcept {
	_core.planData.tasksSuccesses.set(stateId_);

	FFSM2_LOG_TASK_STATUS(_core.context, stateId_, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::fail(const StateID stateId_) noexcept {
	_core.planData.tasksFailures.set(stateId_);

	FFSM2_LOG_TASK_STATUS(_core.context, stateId_, StatusEvent::FAILED);
}

#endif

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::changeTo(const StateID stateId_) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request = Transition{stateId_};

	FFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, stateId_);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateChangeTo(const StateID stateId_) noexcept {
	changeTo(stateId_);

	processRequest();
}
//------------------------------------------------------------------------------
#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::save(SerialBuffer& _buffer) const noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	WriteStream stream{_buffer};

	// TODO: save _core.registry
	// TODO: save _requests
	// TODO: save _rng						// FFSM2_IF_UTILITY_THEORY()
	// TODO: save _core.planData			// FFSM2_IF_PLANS()
	// TODO: save _core.previousTransition	// FFSM2_IF_TRANSITION_HISTORY()
	// TODO: save _activityHistory			// FFSM2_IF_STRUCTURE_REPORT()

	_apex.deepSaveActive(_core.registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::load(const SerialBuffer& buffer) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request.clear();

	PlanControl control{_core};

	_apex.deepExit(control);

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition.clear());

	_core.registry.clearRequests();
	_core.request.clear();

	ReadStream stream{buffer};

	// TODO: load _core.registry
	// TODO: load _requests
	// TODO: load _rng						// FFSM2_IF_UTILITY_THEORY()
	// TODO: load _core.planData			// FFSM2_IF_PLANS()
	// TODO: load _core.previousTransition	// FFSM2_IF_TRANSITION_HISTORY()
	// TODO: load _activityHistory			// FFSM2_IF_STRUCTURE_REPORT()

	_apex.deepLoadRequested(_core.registry, stream);

	_apex.deepEnter(control);
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransition(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.isActive());

	_core.previousTransition.clear();

	if (FFSM2_CHECKED(destination != INVALID_SHORT)) {
		PlanControl control{_core};

		Transition currentTransition;
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

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::initialEnter() noexcept {
	FFSM2_ASSERT(!_core.registry.isActive());
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	PlanControl control{_core};

	Transition currentTransition;
	Transition pendingTransition;

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

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::finalExit() noexcept {
	FFSM2_ASSERT(_core.registry.isActive());
	FFSM2_ASSERT(!_core.request);

	PlanControl control{_core};

	_apex.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processRequest() noexcept {
	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));

	Transition currentTransition;

	if (_core.request) {
		processTransitions(currentTransition);

		FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
	}

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = currentTransition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processTransitions(Transition& currentTransition) noexcept {
	FFSM2_ASSERT(_core.request);

	PlanControl control{_core};

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

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequest(const Transition& currentTransition,
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

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByEntryGuards(const Transition& currentTransition,
								   const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_core
							, currentTransition
							, pendingTransition};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByGuards(const Transition& currentTransition,
							  const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_core
							, currentTransition
							, pendingTransition};

	return _apex.deepForwardExitGuard (guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(Context& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(PureContext&& context
														   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(const RV_& other) noexcept
	: Base{other}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(RV_&& other) noexcept
	: Base{move(other)}
{}

//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(20)
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::~RV_() noexcept {
	finalExit();
}

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.active == INVALID_SHORT);
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	PlanControl control{_core};

	Transition currentTransition;
	applyRequest(currentTransition,
				 destination);

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = Transition{destination});

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId_,
																	const Payload& payload) noexcept
{
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request = Transition{stateId_, payload};

	FFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, stateId_);
}

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::immediateChangeWith(const StateID  stateId_,
																			 const Payload& payload) noexcept
{
	changeWith(stateId_, payload);

	processRequest();
}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::InstanceT(Context& context
																	   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::InstanceT(PureContext&& context
																	   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT, TC*, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::InstanceT(Context context
																		FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT, EmptyContext, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::InstanceT(FFSM2_IF_LOG_INTERFACE(Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

}
}
