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

template <typename TG, typename TA>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::react(const TEvent& event) noexcept {
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

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransition(const StateID destination) noexcept {
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

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::initialEnter() noexcept {
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

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::finalExit() noexcept {
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

	FFSM2_IF_STRUCTURE_REPORT(udpateActivity());
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

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::save(WriteStream& stream) const noexcept {
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

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
	// TODO: save(stream, _activityHistory);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
FFSM2_CONSTEXPR(14)
void
R_<TG, TA>::load(ReadStream& stream) noexcept {
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

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
	// TODO: load(stream, _activityHistory);
#endif

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepChangeToRequested(control);

	FFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

#endif

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

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	stream.template write<1>(1);
	save(stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, TV, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (FFSM2_CHECKED(stream.template read<1>()))
		Base::load(stream);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	if (isActive()) {
		stream.template write<1>(1);

		save(stream);
	}
	else
		stream.template write<1>(0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (stream.template read<1>()) {
		if (isActive())
			Base::load(stream);
		else
			loadEnter (stream);
	}
	else
		if (isActive())
			finalExit();
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.active == INVALID_SHORT);
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	Transition currentTransition;
	PlanControl control{_core, currentTransition};

	applyRequest(currentTransition,
				 destination);

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = Transition{destination});

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT, typename TC, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual, NSL FFSM2_IF_PLANS(, NTC), TP>, TA>::loadEnter(ReadStream& stream) noexcept {
	FFSM2_ASSERT(_core.registry.empty());
	_apex.deepLoadRequested(_core.registry, stream);

	FFSM2_ASSERT(!_core.request);

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_ASSERT(_core.planData.empty() == 0);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	FFSM2_ASSERT(_core.transitionTargets  .empty());
	FFSM2_ASSERT(_core.previousTransitions.empty());
#endif

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
	//FFSM2_ASSERT(_activityHistory.empty());
#endif

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepEnter(control);

	FFSM2_IF_STRUCTURE_REPORT(udpateActivity());
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
