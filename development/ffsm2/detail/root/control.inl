namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId) noexcept
	: control{control_}
	, prevId{control._originId}
{
	control._originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanControlT<TArgs>::Region::Region(PlanControlT& control_) noexcept
	: control  {control_}
{
	control.setRegion();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::setRegion() noexcept
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::resetRegion() noexcept
{
	_status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
FullControlBaseT<TArgs>::Lock::~Lock() noexcept	{
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_core.request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_core.planData.tasksSuccesses.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_core.planData.tasksFailures.set(_originId);

	FFSM2_LOG_TASK_STATUS(context(), _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::updatePlan(TState& headState,
																						  const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it)
				if (_core.registry.active == it->origin &&
					_core.planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					it.remove();

					break;
				}
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);
		}
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId,
																										   const Payload& payload) noexcept
{
	if (!_locked) {
		_core.request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId,
																										   Payload&& payload) noexcept
{
	if (!_locked) {
		_core.request = Transition{_originId, stateId, move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::updatePlan(TState& headState,
																						   const Status subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		if (Plan p = plan()) {
			p.clear();

			_status.result = Status::Result::FAILURE;
			headState.wrapPlanFailed(*this);
		}
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan()) {
			for (auto it = p.first(); it; ++it)
				if (_core.registry.active == it->origin &&
					_core.planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					it.remove();

					break;
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
FFSM2_CONSTEXPR(14)
void
GuardControlT<TArgs>::cancelPendingTransition() noexcept {
	_cancelled = true;

	FFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
