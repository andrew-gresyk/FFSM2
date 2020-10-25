#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

struct TaskBase {
	constexpr TaskBase() noexcept {}

	constexpr TaskBase(const StateID origin_,
					   const StateID destination_) noexcept
		: origin{origin_}
		, destination{destination_}
	{}

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
};

constexpr bool operator == (const TaskBase& lhs, const TaskBase& rhs) noexcept {
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

	constexpr TaskT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TaskT(const StateID origin,
					const StateID destination,
					const Payload& payload) noexcept
		: TaskBase{origin, destination}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr TaskT(const StateID origin,
					const StateID destination,
					Payload&& payload) noexcept
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

	constexpr void clearTaskStatus(const StateID stateId) noexcept;

#ifdef FFSM2_ENABLE_ASSERT
	constexpr void verifyEmptyStatus(const StateID stateId) const noexcept;

	constexpr void verifyPlans() const noexcept;
	constexpr Long verifyPlan()  const noexcept;
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

	constexpr void clearTaskStatus(const StateID stateId) noexcept;

#ifdef FFSM2_ENABLE_ASSERT
	constexpr void verifyEmptyStatus(const StateID stateId) const noexcept;

	constexpr void verifyPlans() const noexcept;
	constexpr Long verifyPlan()  const noexcept;
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"

#endif
