﻿namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
R_<TG, TA>::R_(Context& context
			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger))
	: _context{context}
	FFSM2_IF_LOG_INTERFACE(, _logger{logger})
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() {
	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::update() {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
void
R_<TG, TA>::react(const TEvent& event) {
	FullControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	TransitionSets currentTransitions;

	if (_request.destination != INVALID_SHORT)
		processTransitions(currentTransitions);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::changeTo(const StateID stateId) {
	_request = Transition{stateId};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::initialEnter() {
	FFSM2_ASSERT(_request.destination == INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	_registry.requested = 0;

	TransitionSets currentTransitions;
	Transition pendingTransition;

	cancelledByEntryGuards(currentTransitions,
						   pendingTransition);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _request.destination != INVALID_SHORT;
		 ++i)
	{
		_registry.requested = _request.destination;
		pendingTransition = _request;
		_request.clear();

		if (cancelledByEntryGuards(currentTransitions,
								   pendingTransition))
			FFSM2_BREAK();
		else
			currentTransitions += pendingTransition;

		pendingTransition.clear();
	}

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);

	_registry.clearRequests();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::processTransitions(TransitionSets& currentTransitions) {
	FFSM2_ASSERT(_request.destination != INVALID_SHORT);

	PlanControl control{_context
					  , _registry
					  , _request
					  FFSM2_IF_LOG_INTERFACE(, _logger)};

	Transition pendingTransition;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _request.destination != INVALID_SHORT;
		++i)
	{
		_registry.requested = _request.destination;
		pendingTransition = _request;
		_request.clear();

		if (cancelledByGuards(currentTransitions, pendingTransition))
			;
		else
			currentTransitions += pendingTransition;

		pendingTransition.clear();
	}

	if (currentTransitions.count())
		_apex.deepChangeToRequested(control);

	_registry.clearRequests();
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
								   const Transition& pendingTransition)
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByGuards(const TransitionSets& currentTransitions,
							  const Transition& pendingTransition)
{
	GuardControl guardControl{_context
							, _registry
							, _request
							, currentTransitions
							, pendingTransition
							FFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT, typename TC, Long NSL, typename TP, typename TA>
void
RP_<G_<NFT, TC, NSL, TP>, TA>::changeWith(const StateID  stateId,
										  const Payload& payload)
{
	_request = Transition{stateId, payload};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

template <FeatureTag NFT, typename TC, Long NSL, typename TP, typename TA>
void
RP_<G_<NFT, TC, NSL, TP>, TA>::changeWith(const StateID  stateId,
										  Payload&& payload)
{
	_request = Transition{stateId, std::move(payload)};

	FFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, stateId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
