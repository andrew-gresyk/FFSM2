namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::update() noexcept {
	FFSM2_ASSERT(_registry.isActive());

	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Apex::deepUpdate(control);

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

	Transition currentTransition;

	if (_request)
		processTransitions(currentTransition);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::react(const TEvent& event) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Apex::deepReact(control, event);

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

	Transition currentTransition;

	if (_request)
		processTransitions(currentTransition);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::changeTo(const StateID stateId) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::save(SerialBuffer& _buffer) const noexcept {
	FFSM2_ASSERT(_registry.isActive());

	WriteStream stream{_buffer};

	// TODO: save _registry
	// TODO: save _requests
	// TODO: save _rng						// FFSM2_IF_UTILITY_THEORY()
	// TODO: save _planData					// FFSM2_IF_PLANS()
	// TODO: save _previousTransition		// FFSM2_IF_TRANSITION_HISTORY()
	// TODO: save _activityHistory			// FFSM2_IF_STRUCTURE_REPORT()

	Apex::deepSaveActive(_registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::load(const SerialBuffer& buffer) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_request.clear();

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Apex::deepExit(control);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition.clear());

	_registry.clearRequests();
	_request.clear();

	ReadStream stream{buffer};

	// TODO: load _registry
	// TODO: load _requests
	// TODO: load _rng					// FFSM2_IF_UTILITY_THEORY()
	// TODO: load _planData				// FFSM2_IF_PLANS()
	// TODO: load _previousTransition	// FFSM2_IF_TRANSITION_HISTORY()
	// TODO: load _activityHistory		// FFSM2_IF_STRUCTURE_REPORT()

	Apex::deepLoadRequested(_registry, stream);

	Apex::deepEnter(control);
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransition(const StateID destination) noexcept {
	FFSM2_ASSERT(_registry.isActive());

	_previousTransition.clear();

	if (FFSM2_CHECKED(destination != INVALID_SHORT)) {
		PlanControl control{_context
						  , _registry
						  , _request
						  FFSM2_IF_PLANS(, _planData)
						  , _previousTransition
						  FFSM2_IF_LOG_INTERFACE(, _logger)};

		applyRequest(destination);
		_previousTransition = Transition{destination};

		Apex::deepChangeToRequested(control);

		_registry.clearRequests();

		FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));

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
	FFSM2_ASSERT(!_registry.isActive());
	FFSM2_ASSERT(!_request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_previousTransition));

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	applyRequest(0);

	Transition currentTransition;
	Transition pendingTransition;

	cancelledByEntryGuards(currentTransition,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _request;
		 ++i)
	{
		//backup();

		applyRequest(_request.destination);
		pendingTransition = _request;
		_request.clear();

		if (cancelledByEntryGuards(currentTransition,
								   pendingTransition))
			FFSM2_BREAK();
		else
			currentTransition = pendingTransition;

		pendingTransition.clear();
	}
	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = currentTransition);

	Apex::deepEnter(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::finalExit() noexcept {
	FFSM2_ASSERT(_registry.isActive());
	FFSM2_ASSERT(!_request);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Apex::deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processTransitions(Transition& currentTransition) noexcept {
	FFSM2_ASSERT(_request);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Transition pendingTransition;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _request;
		++i)
	{
		//backup();

		applyRequest(_request.destination);
		pendingTransition = _request;
		_request.clear();

		if (cancelledByGuards(currentTransition, pendingTransition))
			;
		else
			currentTransition = pendingTransition;

		pendingTransition.clear();
	}

	if (currentTransition)
		Apex::deepChangeToRequested(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::applyRequest(const StateID destination) noexcept
{
	_registry.requested = destination;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByEntryGuards(const Transition& currentTransition,
								   const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransition
							, pendingTransition
							FFSM2_IF_PLANS(, _planData)
							FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return Apex::deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByGuards(const Transition& currentTransition,
							  const Transition& pendingTransition) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransition
							, pendingTransition
							FFSM2_IF_PLANS(, _planData)
							FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return Apex::deepForwardExitGuard(guardControl) ||
		   Apex::deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const StateID destination) noexcept {
	FFSM2_ASSERT(_registry.active == INVALID_SHORT);
	FFSM2_ASSERT(!_request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_previousTransition));

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_PLANS(, _planData)
					  FFSM2_IF_TRANSITION_HISTORY(, _previousTransition)
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	applyRequest(destination);

	FFSM2_IF_TRANSITION_HISTORY(_previousTransition = Transition{destination});

	Apex::deepEnter(control);

	_registry.clearRequests();

	FFSM2_IF_PLANS(FFSM2_IF_ASSERT(_planData.verifyPlans()));
}

#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																	const Payload& payload) noexcept
{
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId, payload};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																		 Payload&& payload) noexcept
{
	FFSM2_ASSERT(_registry.isActive());

	_request = Transition{stateId, move(payload)};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

// COMMON
//------------------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

////////////////////////////////////////////////////////////////////////////////

}
}
