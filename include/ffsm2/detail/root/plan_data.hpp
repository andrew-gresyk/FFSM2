#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

struct TaskBase {
	FFSM2_INLINE TaskBase() {}

	FFSM2_INLINE TaskBase(const StateID origin_,
						  const StateID destination_)
		: origin{origin_}
		, destination{destination_}
	{}

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
};

inline bool operator == (const TaskBase& lhs, const TaskBase& rhs) {
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination;
}

//------------------------------------------------------------------------------

template <typename TPayload>
struct TaskT
	: TaskBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 2>::type;

	using TaskBase::TaskBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TaskT() {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TaskT(const StateID origin,
					   const StateID destination,
					   const Payload& payload)
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE TaskT(const StateID origin,
					   const StateID destination,
					   Payload&& payload)
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	bool payloadSet = false;
	Storage storage;
};

//------------------------------------------------------------------------------

template <>
struct TaskT<void>
	: TaskBase
{
	using TaskBase::TaskBase;
};

//------------------------------------------------------------------------------

struct TaskLink {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename
		, typename
		, typename
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
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>>
{
	using StateList		= TStateList;
	using Payload		= TPayload;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<TPayload>;
	using Tasks			= List<Task, TASK_CAPACITY>;
	using TaskLinks		= StaticArray<TaskLink, TASK_CAPACITY>;
	using Payloads		= StaticArray<Payload, TASK_CAPACITY>;

	using TasksBits		= BitArray<StateID, StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	void clearTaskStatus(const StateID stateId);

#ifdef FFSM2_ENABLE_ASSERT
	void verifyEmptyStatus(const StateID stateId) const;

	void verifyPlans() const;
	Long verifyPlan() const;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>>
{
	using StateList		= TStateList;

	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= List<Task, TASK_CAPACITY>;
	using TaskLinks		= StaticArray<TaskLink, TASK_CAPACITY>;

	using TasksBits		= BitArray<StateID, StateList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;

	void clearTaskStatus(const StateID stateId);

#ifdef FFSM2_ENABLE_ASSERT
	void verifyEmptyStatus(const StateID stateId) const;

	void verifyPlans() const;
	Long verifyPlan() const;
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"

#endif
