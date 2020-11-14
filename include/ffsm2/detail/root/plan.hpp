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

	inline Status(const Result result_ = Result::NONE) noexcept;

	constexpr explicit operator bool() const noexcept		{ return result != Result::NONE;	}

	inline void clear() noexcept;
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) noexcept {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef FFSM2_ENABLE_PLANS

template <typename TArgs>
class CPlanT {
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
		FFSM2_INLINE Iterator(const CPlanT& plan)		noexcept;

		FFSM2_INLINE explicit operator bool() const		noexcept;

		FFSM2_INLINE void operator ++()					noexcept;

		FFSM2_INLINE const Task& operator  *() const	noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const	noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next() const					noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_INLINE CPlanT(const PlanData& planData)		noexcept;

	template <typename T>
	static constexpr StateID  stateId()					noexcept	{ return			index<StateList , T>();	}

public:
	FFSM2_INLINE explicit operator bool() const			noexcept;

	FFSM2_INLINE Iterator first()						noexcept	{ return Iterator{*this};					}

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
		FFSM2_INLINE Iterator(PlanBaseT& plan)			noexcept;

		FFSM2_INLINE explicit operator bool() const		noexcept;

		FFSM2_INLINE void operator ++()					noexcept;

		FFSM2_INLINE		Task& operator  *()			noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *() const	noexcept	{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE		Task* operator ->()			noexcept	{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const	noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE void remove()						noexcept;

		FFSM2_INLINE Long next() const					noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator {
		FFSM2_INLINE CIterator(const PlanBaseT& plan)	noexcept;

		FFSM2_INLINE explicit operator bool() const		noexcept;

		FFSM2_INLINE void operator ++()					noexcept;

		FFSM2_INLINE		Task& operator  *()			noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task& operator  *() const	noexcept	{ return  _plan._planData.tasks[_curr];		}

		FFSM2_INLINE		Task* operator ->()			noexcept	{ return &_plan._planData.tasks[_curr];		}
		FFSM2_INLINE const Task* operator ->() const	noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_INLINE Long next() const					noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_INLINE PlanBaseT(PlanData& planData)			noexcept;

	template <typename T>
	static constexpr StateID  stateId()					noexcept	{ return			index<StateList , T>();	}

	FFSM2_INLINE bool append(const StateID origin,
							 const StateID destination)	noexcept;

	FFSM2_INLINE bool linkTask(const Long index)		noexcept;

public:
	FFSM2_INLINE explicit operator bool() const			noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_INLINE void clear()							noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool change(const StateID origin, const StateID destination)	noexcept	{ return append(origin, destination);							}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool change(const StateID destination)							noexcept	{ return change(stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool change()													noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	FFSM2_INLINE  Iterator first()												noexcept	{ return  Iterator{*this};										}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_INLINE CIterator first() const										noexcept	{ return CIterator{*this};										}

private:
	void remove(const Long task)												noexcept;

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
				const Payload& payload) noexcept;

	bool append(const StateID origin,
				const StateID destination,
				Payload&& payload) noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith(const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append	   (origin								 , destination								 ,  		 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_INLINE bool changeWith(const StateID origin, const StateID destination,	   Payload&& payload) noexcept	{ return append	   (origin								 , destination								 , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith(					   const StateID destination, const Payload& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), destination								 , 			 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_INLINE bool changeWith(					   const StateID destination,	   Payload&& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), destination								 , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith(												  const Payload& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), 			 payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_INLINE bool changeWith(													   Payload&& payload) noexcept	{ return changeWith(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), std::move(payload));	}

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
