namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Origin::Origin(ConstControlT& control_,
									 const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	FFSM2_ASSERT(stateId_ < StateList::SIZE || stateId_ == INVALID_STATE_ID);
	control._originId = stateId_;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	FFSM2_ASSERT(stateId_ < StateList::SIZE || stateId_ == INVALID_STATE_ID);
	control._originId = stateId_;
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
	_taskStatus.clear();
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
FullControlBaseT<TArgs>::changeTo(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.request = Transition{_originId, stateId_};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId_);
	}
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::SUCCESS;

	_core.planData.tasksSuccesses.set(stateId_);

	FFSM2_LOG_TASK_STATUS(context(), stateId_, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::FAILURE;

	_core.planData.tasksFailures.set(stateId_);

	FFSM2_LOG_TASK_STATUS(context(), stateId_, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::updatePlan(TState& headState,
																						  const TaskStatus subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan()) {
			for (auto it = p.first();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, it->origin};

					if (const Payload* const payload = it->payload())
						changeWith(it->destination, *it->payload());
					else
						changeTo  (it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					it.remove();

					break;
				}
			}
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();
		}
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL FFSM2_IF_PLANS(, Long NTC), typename TTP>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL FFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId_,
																										   const Payload& payload) noexcept
{
	if (!_locked) {
		_core.request = Transition{_originId, stateId_, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::updatePlan(TState& headState,
																						   const TaskStatus subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan()) {
			for (auto it = p.first();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, it->origin};
					changeTo(it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					it.remove();

					break;
				}
			}
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();
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
