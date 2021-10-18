namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status final {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	FFSM2_CONSTEXPR(11)	Status(const Result result_ = Result::NONE)	  noexcept
		: result{result_}
	{}

	FFSM2_CONSTEXPR(11)	explicit operator bool()				const noexcept	{ return result != Result::NONE;	}

	FFSM2_CONSTEXPR(14)	void clear()								  noexcept	{ result = Result::NONE;			}
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FFSM2_CONSTEXPR(14)
Status
combine(const Status lhs, const Status rhs)							  noexcept	{
	const Status::Result result = lhs.result > rhs.result ?
									  lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

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

	//----------------------------------------------------------------------

	struct IteratorT final {
		FFSM2_CONSTEXPR(14)	IteratorT(const CPlanT& plan)		  noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()		const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()					  noexcept;

		FFSM2_CONSTEXPR(11)	const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()						const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	//----------------------------------------------------------------------

private:
	FFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData)		  noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds}
	{}

	template <typename T>
	static constexpr StateID  stateId()							  noexcept	{ return			index<StateList , T>();	}

public:
	FFSM2_CONSTEXPR(14)	explicit operator bool()			const noexcept;

	FFSM2_CONSTEXPR(11)	IteratorT first()						  noexcept	{ return IteratorT{*this};					}

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

	struct IteratorT final {
		FFSM2_CONSTEXPR(14)	IteratorT(PlanBaseT& plan)			  noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()		const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()					  noexcept;

		FFSM2_CONSTEXPR(14)	Task& operator  *()					  noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(14)	Task* operator ->()					  noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	void remove()						  noexcept	{ _plan.remove(_curr);						}

		FFSM2_CONSTEXPR(14)	Long next()						const noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator final {
		FFSM2_CONSTEXPR(14)	CIterator(const PlanBaseT& plan)	  noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()		const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()					  noexcept;

		FFSM2_CONSTEXPR(14)	const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()						const noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_CONSTEXPR(11)	PlanBaseT(PlanData& planData)			  noexcept;

	template <typename T>
	static constexpr StateID  stateId()							  noexcept	{ return		index<StateList , T>();		}

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination)	  noexcept;

	FFSM2_CONSTEXPR(14)	bool linkTask(const Long index)			  noexcept;

public:
	FFSM2_CONSTEXPR(14)	explicit operator bool()			const noexcept;

	/// @brief Clear all tasks from the plan
	FFSM2_CONSTEXPR(14)	void clear()							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool change(const StateID origin, const StateID destination)  noexcept	{ return append(origin			  , destination)			;	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool change(const StateID destination)						  noexcept	{ return change(stateId<TOrigin>(), destination)			;	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool change()												  noexcept	{ return change(stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return IteratorT to the first task
	FFSM2_CONSTEXPR(14)	 IteratorT first()											  noexcept	{ return  IteratorT{*this};										}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_CONSTEXPR(11)	CIterator first()										const noexcept	{ return CIterator{*this};										}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_CONSTEXPR(14)	void remove(const Long task)								  noexcept;

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
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				FFSM2_IF_SERIALIZATION(, NSerialBits)
				, NSubstitutionLimit
				, NTaskCapacity
				, TPayload>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   FFSM2_IF_SERIALIZATION(, NSerialBits)
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
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									const Payload& payload)									  noexcept;

	FFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									Payload&& payload)										  noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool changeWith(const StateID origin, const StateID destination, const Payload & payload) noexcept	{ return append(origin								 , destination								 ,		payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool changeWith(const StateID origin, const StateID destination,	   Payload&& payload) noexcept	{ return append(origin								 , destination								 , move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool changeWith(					  const StateID destination, const Payload & payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 ,		payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool changeWith(					  const StateID destination,	   Payload&& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool changeWith(												 const Payload & payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(),		payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool changeWith(													   Payload&& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				FFSM2_IF_SERIALIZATION(, NSerialBits)
				, NSubstitutionLimit
				, NTaskCapacity
				, void>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   FFSM2_IF_SERIALIZATION(, NSerialBits)
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
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
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
