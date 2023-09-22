namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ConstControlT {
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;

	using Core				= CoreT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		FFSM2_CONSTEXPR(14)	Origin(ConstControlT& control_,
								   const StateID stateId_)				noexcept;

		FFSM2_CONSTEXPR(20)	~Origin()									noexcept;

		ConstControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	ConstControlT(const Core& core)					noexcept
		: _core{core}
	{}

public:

#if FFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	FFSM2_CONSTEXPR(11) StateID stateId()						  const noexcept	{ return _originId;							}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									noexcept	{ return index<StateList , TState>();		}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(11)	const Context& _()						  const noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(11)	const Context& context()				  const noexcept	{ return _core.context;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11)	const Transition& request()				  const noexcept	{ return _core.request;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)  const noexcept	{ return _core.registry.active == stateId_;	}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive	()						  const noexcept	{ return isActive(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	FFSM2_CONSTEXPR(11)	CPlan plan()							  const noexcept	{ return CPlan{_core.planData};				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransition()'
	/// @return Array of last transition requests
	FFSM2_CONSTEXPR(11)	const Transition& previousTransitions()	  const noexcept	{ return _core.previousTransition;			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	const Core& _core;
	StateID _originId = INVALID_STATE_ID;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;

	using Core				= CoreT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		FFSM2_CONSTEXPR(14)	Origin(ControlT& control_,
								   const StateID stateId_)				noexcept;

		FFSM2_CONSTEXPR(20)	~Origin()									noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	ControlT(Core& core)							noexcept
		: _core{core}
	{}

public:

#if FFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	constexpr StateID stateId()									  const noexcept	{ return _originId;							}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									noexcept	{ return index<StateList , TState>();		}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(14)		  Context& _()								noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(11)	const Context& _()						  const noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(14)		  Context& context()						noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(11)	const Context& context()				  const noexcept	{ return _core.context;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11)	const Transition& request()				  const noexcept	{ return _core.request;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)  const noexcept	{ return _core.registry.active == stateId_;	}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive	()						  const noexcept	{ return isActive(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	FFSM2_CONSTEXPR(11)	CPlan plan()							  const noexcept	{ return CPlan{_core.planData};				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransition()'
	/// @return Array of last transition requests
	FFSM2_CONSTEXPR(11)	const Transition& previousTransitions()	  const noexcept	{ return _core.previousTransition;			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	Core& _core;
	StateID _originId = INVALID_STATE_ID;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	template <typename, typename>
	friend class RV_;

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;
	using typename Control::Core;

	using Transition	= typename Core::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		FFSM2_CONSTEXPR(14)	Region(PlanControlT& control)					noexcept;

		FFSM2_CONSTEXPR(20)	~Region()										noexcept;

		PlanControlT& control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	PlanControlT(Core& core,
									 const Transition& currentTransition)	noexcept
		: Control{core}
		, _currentTransition{currentTransition}
	{}

	FFSM2_CONSTEXPR(14)	void   setRegion()									noexcept;
	FFSM2_CONSTEXPR(14)	void resetRegion()									noexcept;

public:
	using Control::isActive;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;

	/// @brief Access plan
	/// @return Plan
	FFSM2_CONSTEXPR(14)	  Plan plan()										noexcept	{ return  Plan{_core.planData};		}

// COMMON
// COMMON

	/// @brief Access read-only plan
	/// @return Read-only plan
	FFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.planData};		}

// COMMON
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get current transition request
	/// @return Current transition request
	FFSM2_CONSTEXPR(11)	const Transition& currentTransition()		  const noexcept	{ return _currentTransition;	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_core;

	const Transition& _currentTransition;
	TaskStatus _taskStatus;
};

//------------------------------------------------------------------------------

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

	using PlanControl::isActive;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	/// @param stateId State identifier
	FFSM2_CONSTEXPR(14)	void changeTo(const StateID stateId_)			noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo()									noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	FFSM2_CONSTEXPR(14)	void succeed()									noexcept	{ succeed  (_originId							   );	}

	/// @brief Succeed a plan task for a state
	/// @param stateId State ID
	FFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)			noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_CONSTEXPR(14) void succeed()									noexcept	{ succeed  (PlanControl::template stateId<TState>());	}

	/// @brief Fail a plan task for the current state
	FFSM2_CONSTEXPR(14)	void fail	()									noexcept	{ fail	   (_originId							   );	}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	FFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)			noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_CONSTEXPR(14) void fail	()									noexcept	{ fail	   (PlanControl::template stateId<TState>());	}

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

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   FFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  FFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  FFSM2_IF_PLANS(, NTaskCapacity)
								  , TPayload>>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 FFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>;

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
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)			noexcept	{ changeWith(FullControlBase::template stateId<TState>(),	   payload );	}

	// COMMON
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

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   FFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  FFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  FFSM2_IF_PLANS(, NTaskCapacity)
								  , void>>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 FFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 FFSM2_IF_PLANS(, NTaskCapacity)
					 , void>;

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

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Context;
	using typename FullControl::Core;

	using typename FullControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	GuardControlT(Core& core,
									  const Transition& currentTransition,
									  const Transition& pendingTransition) noexcept
		: FullControl{core, currentTransition}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get pending transition request
	/// @return Pending transition request
	FFSM2_CONSTEXPR(11)	const Transition& pendingTransition()	  const noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition request
	///   (can be used to substitute a transition into the current state with a different one)
	FFSM2_CONSTEXPR(14)	void cancelPendingTransition()					noexcept;

private:
	using FullControl::_core;
	using FullControl::_originId;

	const Transition& _pendingTransition;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
