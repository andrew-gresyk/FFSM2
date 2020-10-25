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
	using TransitionSets	= Array<Transition, TArgs::SUBSTITUTION_LIMIT>;

#ifdef FFSM2_ENABLE_PLANS
	using PlanData			= PlanDataT<TArgs>;
	using ConstPlan			= ConstPlanT<TArgs>;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		constexpr Origin(ControlT& control_,
						 const StateID stateId) noexcept;

		~Origin() noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr ControlT(Context& context
					 , Registry& registry
					 , Transition& request
					 FFSM2_IF_PLANS(, PlanData& planData)
					 FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: _context{context}
		, _registry{registry}
		, _request{request}
		FFSM2_IF_PLANS(, _planData{planData})
		FFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()			noexcept	{ return index<StateList, TState>();		}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	constexpr		Context& _()				noexcept	{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	constexpr const Context& _() const			noexcept	{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	constexpr		Context& context()			noexcept	{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	constexpr const Context& context() const	noexcept	{ return _context;							}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	constexpr const Transition& request() const	noexcept	{ return _request;							}

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Access read-only plan
	/// @return Plan
	constexpr ConstPlan plan() const			noexcept	{ return ConstPlan{_planData};				}

#endif

	//----------------------------------------------------------------------

protected:
#ifdef FFSM2_ENABLE_LOG_INTERFACE
	constexpr Logger* logger()					noexcept	{ return _logger;							}
#endif

protected:
	Context& _context;
	Registry& _registry;
	Transition& _request;
	StateID _originId = INVALID_STATE_ID;
	FFSM2_IF_PLANS(PlanData& _planData);
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

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;

#ifdef FFSM2_ENABLE_PLANS
	using typename Control::PlanData;
	using typename Control::ConstPlan;

	using Plan			= PlanT<TArgs>;
#endif

	using Control::Control;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Access plan
	/// @return Plan
	constexpr	   Plan plan()					noexcept	{ return	  Plan{_planData};				}

	/// @brief Access read-only plan
	/// @return Read-only plan
	constexpr ConstPlan plan() const			noexcept	{ return ConstPlan{_planData};				}

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

#ifdef FFSM2_ENABLE_PLANS
	using TasksBits		= BitArray<StateID, StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		constexpr Lock(FullControlBaseT& control_) noexcept;
		~Lock() noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// Clang trips over 'stateId<>()', so give it a hint it comes from PlanControl

	/// @brief Transition into a state
	/// @param stateId State identifier
	constexpr void changeTo (const StateID stateId) noexcept;

	/// @brief Transition into a state
	/// @tparam TState State type
	template <typename TState>
	constexpr void changeTo () noexcept		{ changeTo (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

	/// @brief Succeed a plan task for the current state
	constexpr void succeed() noexcept;

	/// @brief Fail a plan task for the current state
	constexpr void fail()	 noexcept;

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
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
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
					 , NSubstitutionLimit
					 FFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef FFSM2_ENABLE_PLANS

	template <typename TState>
	constexpr void updatePlan(TState& headState, const Status subStatus) noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::changeTo;

	FFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	constexpr void changeWith   (const StateID  stateId,
								 const Payload& payload) noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	constexpr void changeWith   (const StateID  stateId,
									  Payload&& payload) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	constexpr void changeWith   (const Payload& payload) noexcept	{ changeWith   (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	constexpr void changeWith   (	  Payload&& payload) noexcept	{ changeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}

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
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , NSubstitutionLimit
					   FFSM2_IF_PLANS(, NTaskCapacity)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
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
					 , NSubstitutionLimit
					 FFSM2_IF_PLANS(, NTaskCapacity)
					 , void>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef FFSM2_ENABLE_PLANS

	template <typename TState>
	constexpr void updatePlan(TState& headState, const Status subStatus) noexcept;

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
	using typename FullControl::TransitionSets;

#ifdef FFSM2_ENABLE_PLANS
	using typename FullControl::PlanData;
#endif

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr GuardControlT(Context& context
						  , Registry& registry
						  , Transition& request
						  , const TransitionSets& currentTransitions
						  , const Transition& pendingTransition
						  FFSM2_IF_PLANS(, PlanData& planData)
						  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, request
					FFSM2_IF_PLANS(, planData)
					FFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransitions{currentTransitions}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr const TransitionSets& currentTransitions() const	noexcept	{ return _currentTransitions;	}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	constexpr Transition& pendingTransition() const				noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	constexpr void cancelPendingTransition()					noexcept;

private:
	using FullControl::_registry;
	using FullControl::_originId;

	FFSM2_IF_LOG_INTERFACE(using FullControl::_logger);

	const TransitionSets& _currentTransitions;
	const Transition& _pendingTransition;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
