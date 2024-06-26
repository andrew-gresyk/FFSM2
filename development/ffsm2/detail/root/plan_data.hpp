namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskStatus final {
	enum Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	FFSM2_CONSTEXPR(11)	TaskStatus(const Result result_ = Result::NONE)	noexcept;

	FFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	FFSM2_CONSTEXPR(14)	void clear()									noexcept;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

FFSM2_CONSTEXPR(14) TaskStatus  operator |  (TaskStatus& l, const TaskStatus r)	noexcept;
FFSM2_CONSTEXPR(14) TaskStatus& operator |= (TaskStatus& l, const TaskStatus r)	noexcept;

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_PLANS_AVAILABLE()

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;

	FFSM2_CONSTEXPR(14) void clear()									noexcept	{ first = INVALID_LONG; last = INVALID_LONG;	}
};

////////////////////////////////////////////////////////////////////////////////

template <
	typename
  , typename
  FFSM2_IF_SERIALIZATION(, Long)
  , Long
  , typename
>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
  , typename TPayload
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 FFSM2_IF_SERIALIZATION(, NSerialBits)
			 , NTaskCapacity
			 , TPayload
		   >
	   > final
{
	using StateList			= TStateList;
	using Payload			= TPayload;

	static constexpr Long STATE_COUNT	= StateList ::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <Payload>;
	using Tasks				= TaskListT   <Payload,  TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads			= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBits			= BitArrayT   <				STATE_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;
	TaskStatus headStatus;
	TaskStatus subStatus;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	FFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	FFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()							  const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 FFSM2_IF_SERIALIZATION(, NSerialBits)
			 , NTaskCapacity
			 , void
		   >
	   > final
{
	using StateList			= TStateList;

	static constexpr Long  STATE_COUNT	= StateList ::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <void>;
	using Tasks				= TaskListT	  <void,	 TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBits			= BitArrayT	  <				STATE_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;

	Bounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	bool planExists;
	TaskStatus headStatus;
	TaskStatus subStatus;

	FFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	FFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	FFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	FFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if FFSM2_ASSERT_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{ FFSM2_ASSERT(tasks.count() == verifyPlan());	}
	FFSM2_CONSTEXPR(14)	Long verifyPlan()							  const noexcept;
#endif
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"
