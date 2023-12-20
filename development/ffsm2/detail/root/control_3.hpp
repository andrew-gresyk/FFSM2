namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class FullControlBaseT
	: public PlanControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using PlanControl	= PlanControlT<TArgs>;

	using typename PlanControl::StateList;
	using typename PlanControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using TasksBits		= BitArrayT<StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock final {
		FFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)			noexcept;
		FFSM2_CONSTEXPR(20)	~Lock()										noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	/// @param `stateId` State identifier
	FFSM2_CONSTEXPR(14)	void changeTo(const StateID stateId_)			noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` State type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo()									noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	void succeed()								noexcept	{ succeed  (_originId							   );	}

	/// @brief Succeed a plan task for a state
	/// @param `stateId` State identifier
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)		noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void succeed()								noexcept	{ succeed  (PlanControl::template stateId<TState>());	}

	/// @brief Fail a plan task for the current state
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	void fail	()								noexcept	{ fail	   (_originId							   );	}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)		noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void fail	()								noexcept	{ fail	   (PlanControl::template stateId<TState>());	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_core;
	using PlanControl::_originId;

	using PlanControl::_taskStatus;

	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
class FullControlT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			FFSM2_IF_PLANS(, NTaskCapacity)
			, TPayload
		  >
	  >
	: public FullControlBaseT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , TPayload
				 >
			 >
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , TPayload
				 >;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#if FFSM2_PLANS_AVAILABLE()

	template <typename TState>
	FFSM2_CONSTEXPR(14)	void updatePlan(TState& headState,
										const TaskStatus subStatus)		noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)			noexcept	{ changeWith(FullControlBase::template stateId<TState>(),	   payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

protected:
	using FullControlBase::_core;
	using FullControlBase::_originId;

	using FullControlBase::_taskStatus;

	using FullControlBase::_locked;
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  FFSM2_IF_PLANS(, Long NTaskCapacity)
>
class FullControlT<
		  ArgsT<
			  TConfig
			, TStateList
			FFSM2_IF_SERIALIZATION(, NSerialBits)
			FFSM2_IF_PLANS(, NTaskCapacity)
			, void
		  >
	  >
	: public FullControlBaseT<
				 ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , void
				 >
			 >
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   FFSM2_IF_SERIALIZATION(, NSerialBits)
				   FFSM2_IF_PLANS(, NTaskCapacity)
				   , void
				 >;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#if FFSM2_PLANS_AVAILABLE()

	template <typename TState>
	FFSM2_CONSTEXPR(14)	void updatePlan(TState& headState,
										const TaskStatus subStatus)		noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	FFSM2_IF_PLANS(using FullControlBase::_core);

	using FullControlBase::_taskStatus;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_3.inl"
