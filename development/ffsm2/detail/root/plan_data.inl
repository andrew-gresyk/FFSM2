namespace ffsm2 {
namespace detail {

//------------------------------------------------------------------------------

FFSM2_CONSTEXPR(14)
void
Status::clear() noexcept {
	result = Result::NONE;
}

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearRegionStatuses() noexcept {
	subStatus .clear();
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlan() const noexcept	{
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

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyEmptyStatus(const StateID FFSM2_IF_ASSERT(stateId)) const noexcept {
#if FFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		FFSM2_ASSERT(!tasksSuccesses.get(stateId));
		FFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearRegionStatuses() noexcept {
	subStatus .clear();
}

//------------------------------------------------------------------------------

#if FFSM2_ASSERT_AVAILABLE()

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL FFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
FFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL FFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlan() const noexcept	{
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
