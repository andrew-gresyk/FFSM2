namespace ffsm2 {
namespace detail {

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

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
	using CPlan				= CPlanT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		FFSM2_CONSTEXPR(14)	Origin(ControlT& control_,
								   const StateID stateId)				  noexcept;

		FFSM2_CONSTEXPR(20) ~Origin()									  noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	ControlT(Context& context
							   , Registry& registry
							   , Transition& request
							   FFSM2_IF_PLANS(, PlanData& planData)
							   FFSM2_IF_TRANSITION_HISTORY(, const Transition& previousTransition)
							   FFSM2_IF_LOG_INTERFACE(, Logger* const logger))		  noexcept
		: _context{context}
		, _registry{registry}
		, _request{request}
		FFSM2_IF_PLANS(, _planData{planData})
		FFSM2_IF_TRANSITION_HISTORY(, _previousTransition{previousTransition})
		FFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									  noexcept	{ return index<StateList , TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(14)		  Context& _()								  noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	FFSM2_CONSTEXPR(11) const Context& _()							const noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(14)		  Context& context()						  noexcept	{ return _context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	FFSM2_CONSTEXPR(11) const Context& context()					const noexcept	{ return _context;						}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11) const Transition& request()					const noexcept	{ return _request;						}

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	FFSM2_CONSTEXPR(11) CPlan plan()								const noexcept	{ return CPlan{_planData};				}

#endif

	//----------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransition()'
	/// @return Array of last transition requests
	FFSM2_CONSTEXPR(11) const Transition& previousTransitions()		const noexcept	{ return _previousTransition;			}

#endif

	//----------------------------------------------------------------------

protected:
	Context& _context;
	Registry& _registry;
	Transition& _request;
	StateID _originId = INVALID_STATE_ID;
	FFSM2_IF_PLANS(PlanData& _planData);
	FFSM2_IF_TRANSITION_HISTORY(const Transition& _previousTransition);
	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

//------------------------------------------------------------------------------

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

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;
#endif

	using Control::Control;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()
// COMMON

	/// @brief Access plan
	/// @return Plan
	FFSM2_CONSTEXPR(14)	  Plan plan()							  noexcept	{ return  Plan{_planData};						}

// COMMON
// COMMON

	/// @brief Access read-only plan
	/// @return Read-only plan
	FFSM2_CONSTEXPR(11) CPlan plan()						const noexcept	{ return CPlan{_planData};						}

// COMMON
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_IF_PLANS(using Control::_planData);
	FFSM2_IF_LOG_INTERFACE(using Control::_logger);

	Status _status;
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
		FFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)	  noexcept;
		FFSM2_CONSTEXPR(20)	~Lock()								  noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId State identifier
	FFSM2_CONSTEXPR(14)	void changeTo(const StateID stateId)	  noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo()							  noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	//----------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	//----------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	FFSM2_CONSTEXPR(14)	void succeed()							  noexcept;

	/// @brief Fail a plan task for the current state
	FFSM2_CONSTEXPR(14)	void fail()								  noexcept;

#endif

	//----------------------------------------------------------------------

protected:
	using PlanControl::_request;
	FFSM2_IF_PLANS(using PlanControl::_planData);
	FFSM2_IF_LOG_INTERFACE(using PlanControl::_logger);

	using PlanControl::_originId;
	using PlanControl::_status;

	bool _locked = false;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
class FullControlT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
										const Status subStatus)		  noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)		  noexcept	{ changeWith(FullControlBase::template stateId<TState>(),			payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(Payload&& payload)			  noexcept	{ changeWith(FullControlBase::template stateId<TState>(), move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//------------------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_request;
	FFSM2_IF_PLANS(using FullControlBase::_planData);
	FFSM2_IF_PLANS(using FullControlBase::_registry);
	FFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
										const Status subStatus)	  noexcept;

#endif

public:
	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	FFSM2_IF_PLANS(using FullControlBase::_registry);
	FFSM2_IF_PLANS(using FullControlBase::_planData);

	using FullControlBase::_status;
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

	using typename FullControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11) GuardControlT(Context& context
									, Registry& registry
									, Transition& request
									, const Transition& currentTransition
									, const Transition& pendingTransition
									FFSM2_IF_PLANS(, PlanData& planData)
									FFSM2_IF_TRANSITION_HISTORY(, const Transition& previousTransition)
									FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, request
					FFSM2_IF_PLANS(, planData)
					FFSM2_IF_TRANSITION_HISTORY(, previousTransition)
					FFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransition{currentTransition}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get current transition request
	/// @return Current transition request
	FFSM2_CONSTEXPR(11) const Transition& currentTransitions()	const noexcept	{ return _currentTransition;	}

	/// @brief Get pending transition request
	/// @return Pending transition request
	FFSM2_CONSTEXPR(11)	const Transition& pendingTransition()	const noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition request
	///		(can be used to substitute a transition into the current state with a different one)
	FFSM2_CONSTEXPR(14)	void cancelPendingTransition()				  noexcept;

private:
	using FullControl::_registry;
	using FullControl::_originId;

	FFSM2_IF_LOG_INTERFACE(using FullControl::_logger);

	const Transition& _currentTransition;
	const Transition& _pendingTransition;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
