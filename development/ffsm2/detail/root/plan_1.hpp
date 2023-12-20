#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long STATE_COUNT	= StateList::SIZE;
	static constexpr Long TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator final {
		FFSM2_CONSTEXPR(14)	CIterator(const PlanT& plan)				noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		FFSM2_CONSTEXPR(14)	const Task& operator  *()			  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()			  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		const PlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator final {
		FFSM2_CONSTEXPR(14)	Iterator(PlanT& plan)						noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		FFSM2_CONSTEXPR(14)	Task& operator  *()							noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(14)	Task* operator ->()							noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	void remove()								noexcept	{ _plan.remove(_curr);						}

		FFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		PlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_CONSTEXPR(11)	PlanT(PlanData& planData)						noexcept;

	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()								noexcept	{ return		index<StateList , TState>();		}

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination)			noexcept;

	FFSM2_CONSTEXPR(14)	bool linkTask(const Long index)					noexcept;

	FFSM2_CONSTEXPR(14)	void clearTasks()								noexcept;

public:
	FFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_CONSTEXPR(14)	void clear()									noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool change(const StateID origin,
									const StateID destination)			noexcept	{ return append(origin			  , destination)			;	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool change(const StateID destination)			noexcept	{ return change(stateId<TOrigin>(), destination)			;	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool change()													noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	FFSM2_CONSTEXPR(14)	 Iterator begin()								noexcept	{ return  Iterator{*this};	}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_CONSTEXPR(11)	CIterator begin()						  const noexcept	{ return CIterator{*this};	}

	/// @brief Iteration terminator
	/// @return Dummy Iterator
	FFSM2_CONSTEXPR(14)	 Iterator end()									noexcept	{ return  Iterator{*this};	}

	/// @brief Iteration terminator
	/// @return Dummy CIterator
	FFSM2_CONSTEXPR(11)	CIterator end()							  const noexcept	{ return CIterator{*this};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief First task
	/// @return First task
	FFSM2_CONSTEXPR(14)		  Task& first()								noexcept;

	/// @brief First task
	/// @return First task
	FFSM2_CONSTEXPR(14) const Task& first()						  const noexcept;

	/// @brief Last task
	/// @return Last task
	FFSM2_CONSTEXPR(14)		  Task&  last()								noexcept;

	/// @brief Last task
	/// @return Last task
	FFSM2_CONSTEXPR(14) const Task&  last()						  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_CONSTEXPR(14)	void remove(const Long task)					noexcept;

protected:
	PlanData& _planData;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_1.inl"
