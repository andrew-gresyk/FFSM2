namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
constexpr
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
constexpr
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
constexpr void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
constexpr void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
constexpr void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
template <typename TState>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::updatePlan(TState& headState,
															const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan())
			p.clear();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																			 const Payload& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																				  Payload&& payload) noexcept
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
template <typename TState>
constexpr void
FullControlT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::updatePlan(TState& headState,
															 const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan())
			p.clear();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (_registry.active == it->origin &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
constexpr void
GuardControlT<TArgs>::cancelPendingTransition() noexcept {
	_cancelled = true;

	FFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
