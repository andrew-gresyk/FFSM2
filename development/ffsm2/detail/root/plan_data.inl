namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

FFSM2_CONSTEXPR(11)
TaskStatus::TaskStatus(const Result result_) noexcept
	: result{result_}
{}

//------------------------------------------------------------------------------

FFSM2_CONSTEXPR(11)
TaskStatus::operator bool() const noexcept {
	return result != Result::NONE;
}

//------------------------------------------------------------------------------

FFSM2_CONSTEXPR(14)
void
TaskStatus::clear() noexcept {
	result = Result::NONE;
}

////////////////////////////////////////////////////////////////////////////////

FFSM2_CONSTEXPR(14)
TaskStatus
operator | (TaskStatus& lhs,
			const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return TaskStatus{result};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FFSM2_CONSTEXPR(14)
TaskStatus&
operator |= (TaskStatus& lhs,
			 const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
										  lhs.result : rhs.result;

	lhs = TaskStatus{result};

	return lhs;
}

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clearRegionStatuses() noexcept {
	headStatus.clear();
	 subStatus.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clear() noexcept {
	tasks		  .clear();
	taskLinks	  .clear();
	taskPayloads  .clear();
	payloadExists .clear();

	tasksBounds	  .clear();
	tasksSuccesses.clear();
	tasksFailures .clear();
	planExists	 = false;

	clearRegionStatuses();
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyPlan() const noexcept	{
	Long length = 0;
	const Bounds& bounds = tasksBounds;

	if (bounds.first != INVALID_LONG) {
		FFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (Long slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				FFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					FFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				FFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		FFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clearRegionStatuses() noexcept {
	headStatus.clear();
	subStatus .clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clear() noexcept {
	tasks		  .clear();
	taskLinks	  .clear();

	tasksBounds	  .clear();
	tasksSuccesses.clear();
	tasksFailures .clear();
	planExists	 = false;

	clearRegionStatuses();
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyPlan() const noexcept	{
	Long length = 0;
	const Bounds& bounds = tasksBounds;

	if (bounds.first != INVALID_LONG) {
		FFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				FFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					FFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				FFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		FFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif
#endif

////////////////////////////////////////////////////////////////////////////////

}
}
