namespace ffsm2 {
namespace detail {

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

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::updatePlan(TState& headState,
																					  const TaskStatus subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits successesToClear;
			successesToClear.set();

			for (auto it = p.begin();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, it->origin}; // SPECIFIC

					if (const Payload* const payload = it->payload())
						changeWith(it->destination, *it->payload());
					else
						changeTo  (it->destination);

					if (it->cyclic())
						_core.planData.tasksSuccesses.clear(it->origin); // SPECIFIC
					else
						successesToClear.clear(it->origin);

					it.remove();
				}
			}

			_core.planData.tasksSuccesses &= successesToClear;
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();
		}
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_) FFSM2_IF_PLANS(, Long NTC_), typename TTP_>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_) FFSM2_IF_PLANS(, NTC_), TTP_>>::changeWith(const StateID  stateId_,
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

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
template <typename TState>
FFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::updatePlan(TState& headState,
																					  const TaskStatus subStatus) noexcept
{
	FFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan()) {
			TasksBits successesToClear;
			successesToClear.set();

			for (auto it = p.begin();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, it->origin}; // SPECIFIC

					changeTo(it->destination);

					if (it->cyclic())
						_core.planData.tasksSuccesses.clear(it->origin); // SPECIFIC
					else
						successesToClear.clear(it->origin);

					it.remove();
				}
			}

			_core.planData.tasksSuccesses &= successesToClear;
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
