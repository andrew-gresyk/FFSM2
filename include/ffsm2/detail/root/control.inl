namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId) noexcept
	: control{control_}
	, prevId{control._originId}
{
	control._originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ANY_PLANS

template <typename TArgs>
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	FFSM2_IF_DYNAMIC_PLANS(_planData.tasksSuccesses.set(_originId));

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	FFSM2_IF_DYNAMIC_PLANS(_planData.tasksFailures.set(_originId));

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_ANY_PLANS

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
template <typename TState>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::updatePlan(TState& FFSM2_IF_DYNAMIC_PLANS(headState),
																												  const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}
	#endif

	} else if (subStatus.result == Status::Result::SUCCESS) {
		FFSM2_IF_STATIC_PLANS(bool processed = false);

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it) {
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					it.remove();

					FFSM2_IF_STATIC_PLANS(processed = true);
					break;
				}
			}

			if (!p) {
				_status.result = Status::Result::SUCCESS;
				headState.wrapPlanSucceeded(*this);
			}
		}

	#endif

	#ifdef FFSM2_ENABLE_STATIC_PLANS

		if (!processed) {
			const StateID destination = LinksLookup::get(_registry.active);

			if (destination != INVALID_STATE_ID) {
				Origin origin{*this, _registry.active};
				changeTo(destination);
			}
		}

	#endif
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::changeWith(const StateID  stateId,
																												  const Payload& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), TTP>>::changeWith(const StateID  stateId,
																													   Payload&& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_ANY_PLANS

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_DYNAMIC_PLANS(, Long NTC) FFSM2_IF_STATIC_PLANS(, typename TPL)>
template <typename TState>
void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_DYNAMIC_PLANS(, NTC) FFSM2_IF_STATIC_PLANS(, TPL), void>>::updatePlan(TState& FFSM2_IF_DYNAMIC_PLANS(headState),
																												   const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}

	#endif

	} else if (subStatus.result == Status::Result::SUCCESS) {
		FFSM2_IF_STATIC_PLANS(bool processed = false);

	#ifdef FFSM2_ENABLE_DYNAMIC_PLANS

		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it) {
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					it.remove();

					FFSM2_IF_STATIC_PLANS(processed = true);
					break;
				}
			}

			if (!p) {
				_status.result = Status::Result::SUCCESS;
				headState.wrapPlanSucceeded(*this);
			}
		}

	#endif

	#ifdef FFSM2_ENABLE_STATIC_PLANS

		if (!processed) {
			const StateID destination = LinksLookup::get(_registry.active);

			if (destination != INVALID_STATE_ID) {
				Origin origin{*this, _registry.active};
				changeTo(destination);
			}
		}

	#endif
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
GuardControlT<TArgs>::cancelPendingTransition() noexcept {
	_cancelled = true;

	FFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
