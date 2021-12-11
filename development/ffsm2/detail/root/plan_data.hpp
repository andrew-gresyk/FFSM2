#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename
		, typename
		, typename
		FFSM2_IF_SERIALIZATION(, Long)
		, Long
		, Long
		, typename>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>> final
{
	using StateList		= TStateList;
	using Payload		= TPayload;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<Payload>;
	using Tasks			= TaskListT<Payload, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads		= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBits		= BitArrayT<StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus(const StateID stateId)			  noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId)	const noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()								const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>> final
{
	using StateList		= TStateList;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= TaskListT<void, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBits		= BitArrayT<StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus(const StateID stateId)			  noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId)	const noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()								const noexcept;
#endif
};

//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"

#endif