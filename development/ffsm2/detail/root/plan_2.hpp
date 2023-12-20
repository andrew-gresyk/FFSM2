#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PayloadPlanT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
  , typename TPayload
>
class PayloadPlanT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			, NTaskCapacity
			, TPayload
		  >
	  > final
	: public PlanT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , TPayload
				 >
			 >
{
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , TPayload
				 >;

	using Payload		= typename Args::Payload;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

	using PlanBase		= PlanT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	FFSM2_CONSTEXPR(14)	bool  append(const StateID origin,
									 const StateID destination,
									 const Payload& payload)			noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	FFSM2_CONSTEXPR(14)	bool changeWith   (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 ,		payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	FFSM2_CONSTEXPR(14)	bool changeWith   (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 ,		payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	FFSM2_CONSTEXPR(14)	bool changeWith	  (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(),		payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
>
class PayloadPlanT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			, NTaskCapacity
			, void
		  >
	  > final
	: public PlanT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , void
				 >
			 >
{
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , void
				 >;

	using PlanBase = PlanT<Args>;

	using PlanBase::PlanBase;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_2.inl"
