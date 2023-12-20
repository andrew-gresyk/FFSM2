namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	typename TConfig
  , typename TApex
>
class R_ {
public:
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;
	using Payload				= typename TConfig::Payload;

protected:
	using Forward				= RF_<TConfig, TApex>;
	using StateList				= typename Forward::StateList;

	using Args					= typename Forward::Args;
	using PureContext			= typename Args::PureContext;

	static_assert(Args::STATE_COUNT	   < static_cast<unsigned>(-1), "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT	  == static_cast<unsigned>(StateList::SIZE), "STATE_COUNT != StateList::SIZE");

	using Core					= CoreT<Args>;

	using Apex					= MaterialT<0, Args, TApex>;

	using ConstControl			= ConstControlT<Args>;
	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Short	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

public:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	/// @brief Transition
	using Transition			= typename Core::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using CPlan					= CPlanT<Args>;
	using Plan					= PayloadPlanT<Args>;

	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	explicit R_(Context& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	FFSM2_CONSTEXPR(11)	explicit R_(PureContext&& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	FFSM2_CONSTEXPR(NO) R_(const R_& )															noexcept = default;
	FFSM2_CONSTEXPR(NO) R_(		 R_&&)															noexcept = default;

	FFSM2_CONSTEXPR(20)	~R_()																	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(14)		  Context& context()												noexcept	{ return _core.context;							}

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(11)	const Context& context()										  const noexcept	{ return _core.context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID  stateId()														noexcept	{ return					   index<StateList , TState>() ;	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access state instance
	/// @tparam `TState` State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(14)		  TState& access()													noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam `TState` State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(11)	const TState& access()											  const noexcept	{ return static_cast<const TState&>(_apex);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Trigger FSM update cycle (recursively call `update()` from the root down to the leaf states
	///   on all active states then process requested transitions)
	FFSM2_CONSTEXPR(14)	void update()															noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states
	///   on all active states then process requested transitions)
	/// @tparam `TEvent` Event type
	/// @param `event` Event to react to
	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void react(const TEvent& event)											noexcept;

	/// @brief Recursively call 'query()' from the root down to the leaf states on all active states
	/// @tparam `TEvent` Event type
	/// @param `event` Event to react to
	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void query(TEvent& event)										  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_CONSTEXPR(11)	StateID activeStateId()											  const noexcept	{ return _core.registry.active;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param `stateId` Destination state identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive(const StateID stateId_)							  const noexcept	{ return _core.registry.active == stateId_;				}

	/// @brief Check if a state is active
	/// @tparam `TState` Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive()													  const noexcept	{ return _core.registry.active == stateId<TState>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	  Plan plan()															noexcept	{ return  Plan{_core.planData};		}

	/// @brief Access read-only plan
	/// @return Read-only plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.planData};		}

	/// @brief Succeed a plan task for a state
	/// @param `stateId` State identifier
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)									noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void succeed()															noexcept	{ succeed(stateId<TState>());		}

	/// @brief Fail a plan task for a state
	/// @param `stateId` State identifier
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)									noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam `TState` State type
	/// @see `FFSM2_ENABLE_PLANS`
	template <typename TState>
	FFSM2_CONSTEXPR(14) void fail	()															noexcept	{ fail	 (stateId<TState>());		}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @param `stateId` Destination state identifier
	/// @see `immediateChangeTo()`
	FFSM2_CONSTEXPR(14)	void changeTo			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @tparam `TState` Destination state type
	/// @see `immediateChangeTo()`
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo			()												noexcept	{ changeTo (stateId<TState>());		}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @see `changeTo()`
	FFSM2_CONSTEXPR(14)	void immediateChangeTo	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` Destination state type
	/// @see `changeTo()`
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void immediateChangeTo	()												noexcept	{ immediateChangeTo	(stateId<TState>());			}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the transition recorded during last `update()` / `react()`
	/// @return Array of last recorded transitions
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(11)	const Transition& previousTransition()							  const noexcept	{ return _core.previousTransition;	}

	/// @brief Force process a transition (skips `guard()` calls)
	///   Can be used to synchronize multiple FSMs
	/// @param `transition` 'Transition' to replay
	/// @return Success status
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(14)	bool replayTransition(const StateID destination)						noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param `logger` A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)									noexcept	{ _core.logger = logger;			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	FFSM2_CONSTEXPR(14)	void initialEnter()														noexcept;
	FFSM2_CONSTEXPR(14)	void finalExit()														noexcept;

	FFSM2_CONSTEXPR(14)	void processRequest()													noexcept;
	FFSM2_CONSTEXPR(14)	void processTransitions(Transition& currentTransition)					noexcept;

	FFSM2_CONSTEXPR(14)	bool applyRequest(const Transition& currentTransition,
										  const StateID destination)							noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByEntryGuards(const Transition& currentTransition,
													const Transition& pendingTransition)		noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByGuards(const Transition& currentTransition,
											   const Transition& pendingTransition)				noexcept;

#if FFSM2_SERIALIZATION_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void save(WriteStream& stream)									  const noexcept;
	FFSM2_CONSTEXPR(14)	void load( ReadStream& stream)											noexcept;
#endif

protected:
	Core _core;
	Apex _apex;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_0.inl"
