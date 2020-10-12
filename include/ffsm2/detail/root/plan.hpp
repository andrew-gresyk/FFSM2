namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	inline Status(const Result result_ = Result::NONE);

	inline explicit operator bool() const	{ return result != Result::NONE; }

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
class ConstPlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		FFSM2_INLINE Iterator(const ConstPlanT& plan);

		FFSM2_INLINE explicit operator bool() const;

		FFSM2_INLINE void operator ++();

		FFSM2_INLINE const Task& operator  *() const		{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const		{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next() const;

		const ConstPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_INLINE ConstPlanT(const PlanData& planData);

	template <typename T>
	static constexpr StateID  stateId()						{ return			index<StateList , T>();	}

public:
	FFSM2_INLINE explicit operator bool() const;

	FFSM2_INLINE Iterator first()							{ return Iterator{*this};					}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanBaseT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		FFSM2_INLINE Iterator(PlanBaseT& plan);

		FFSM2_INLINE explicit operator bool() const;

		FFSM2_INLINE void operator ++();

		FFSM2_INLINE	   Task& operator  *()				{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *() const		{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE	   Task* operator ->()				{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const		{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE void remove();

		FFSM2_INLINE Long next() const;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct ConstIterator {
		FFSM2_INLINE ConstIterator(const PlanBaseT& plan);

		FFSM2_INLINE explicit operator bool() const;

		FFSM2_INLINE void operator ++();

		FFSM2_INLINE	   Task& operator  *()				{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *() const		{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE	   Task* operator ->()				{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const		{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next() const;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_INLINE PlanBaseT(PlanData& planData);

	template <typename T>
	static constexpr StateID  stateId()						{ return			index<StateList , T>();	}

	bool append(const StateID origin,
				const StateID destination);

	bool linkTask(const Long index);

public:
	FFSM2_INLINE explicit operator bool() const;

	/// @brief Clear all tasks from the plan
	FFSM2_INLINE void clear();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool change   (const StateID origin, const StateID destination)	{ return append(origin, destination);							}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool change   (const StateID destination)							{ return change(stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool change   ()													{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	FFSM2_INLINE	  Iterator first()												{ return	  Iterator{*this};									}

	/// @brief Begin iteration over plan tasks
	/// @return ConstIterator to the first task
	FFSM2_INLINE ConstIterator first() const										{ return ConstIterator{*this};									}

private:
	void remove(const Long task);

protected:
	PlanData& _planData;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, NSubstitutionLimit
				, NTaskCapacity
				, TPayload>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , TPayload>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	bool append(const StateID origin,
				const StateID destination,
				const Payload& payload);

	bool append(const StateID origin,
				const StateID destination,
				Payload&& payload);

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith   (const StateID origin, const StateID destination, const Payload& payload)	{ return append		  (origin								, destination								,  			payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith   (const StateID origin, const StateID destination,	  Payload&& payload)	{ return append		  (origin								, destination								, std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith   (					  const StateID destination, const Payload& payload)	{ return changeWith   (PlanBase::template stateId<TOrigin>(), destination								, 			payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith   (					  const StateID destination,	  Payload&& payload)	{ return changeWith   (PlanBase::template stateId<TOrigin>(), destination								, std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith   (												 const Payload& payload)	{ return changeWith   (PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), 			payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith   (													  Payload&& payload)	{ return changeWith   (PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, NSubstitutionLimit
				, NTaskCapacity
				, void>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , void>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>;

	using PlanBase = PlanBaseT<Args>;

	using PlanBase::PlanBase;
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan.inl"
