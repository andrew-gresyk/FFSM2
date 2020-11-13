#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::verifyEmptyStatus(const StateID stateId) const noexcept {
	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC, typename TTP>
Long
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, TTP>>::verifyPlan() const noexcept {
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

					if (fast != INVALID_LONG) {
						fast = taskLinks[fast].next;
					}

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

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::verifyEmptyStatus(const StateID stateId) const noexcept {
	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::verifyPlans() const noexcept {
	FFSM2_ASSERT(tasks.count() == verifyPlan());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, Long NTC>
Long
PlanDataT<ArgsT<TC, TG, TSL, NSL, NTC, void>>::verifyPlan() const noexcept {
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

					if (fast != INVALID_LONG) {
						fast = taskLinks[fast].next;
					}

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

}
}

#endif
