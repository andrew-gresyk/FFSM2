#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::CIterator::CIterator(const PlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::CIterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr  < PlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
PlanT<TArgs>::CIterator::next() const noexcept {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::Iterator::Iterator(PlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::Iterator::operator bool() const noexcept {
	FFSM2_ASSERT(_curr < PlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
Long
PlanT<TArgs>::Iterator::next() const noexcept {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		FFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(11)
PlanT<TArgs>::PlanT(PlanData& planData) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
bool
PlanT<TArgs>::append(const StateID origin,
					 const StateID destination) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists = true;

		return linkTask(_planData.tasks.emplace(origin, destination));
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(14)
bool
PlanT<TArgs>::linkTask(const Long index) noexcept	{
	if (index != Tasks::INVALID) {
		if (_bounds.first == INVALID_LONG) {
			FFSM2_ASSERT(_bounds.last == INVALID_LONG);
			FFSM2_ASSERT(_planData.taskLinks[index].prev == INVALID_LONG);
			FFSM2_ASSERT(_planData.taskLinks[index].next == INVALID_LONG);

			_bounds.first = index;
			_bounds.last  = index;
		} else {
			FFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
			FFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

			TaskLink& lastLink = _planData.taskLinks[_bounds.last];
			FFSM2_ASSERT(lastLink.next == INVALID_LONG);

			lastLink.next  = index;

			TaskLink& currLink = _planData.taskLinks[index];
			FFSM2_ASSERT(currLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clearTasks() noexcept	{
	if (_bounds.first < TaskLinks::CAPACITY) {
		FFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			FFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const TaskLink& link = _planData.taskLinks[index];
			FFSM2_ASSERT(index == _bounds.first ?
						 link.prev == INVALID_LONG :
						 link.prev <  TaskLinks::CAPACITY);

			const Long next = link.next;

			remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG;
		_bounds.last  = INVALID_LONG;
	} else {
		FFSM2_ASSERT(_bounds.first == INVALID_LONG);
		FFSM2_ASSERT(_bounds.last  == INVALID_LONG);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
PlanT<TArgs>::operator bool() const noexcept {
	FFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clear() noexcept	{
	clearTasks();

	for (StateID i = 0;
		 i < STATE_COUNT;
		 ++i)
	{
		_planData.tasksSuccesses.clear(i);
		_planData.tasksFailures .clear(i);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::remove(const Long index) noexcept {
	FFSM2_ASSERT(_planData.planExists);
	FFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
	FFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

	FFSM2_ASSERT(index < TaskLinks::CAPACITY);

	TaskLink& link = _planData.taskLinks[index];

	if (link.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[link.prev];
		prev.next = link.next;
	} else {
		FFSM2_ASSERT(_bounds.first == index);
		_bounds.first = link.next;
	}

	if (link.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[link.next];
		next.prev = link.prev;
	} else {
		FFSM2_ASSERT(_bounds.last == index);
		_bounds.last = link.prev;
	}

	link.prev = INVALID_LONG;
	link.next = INVALID_LONG;

	_planData.tasks.remove(index);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
