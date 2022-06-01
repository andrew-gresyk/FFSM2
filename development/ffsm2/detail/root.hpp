namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
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

	static_assert(Args::STATE_COUNT <  static_cast<unsigned>(-1), "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == static_cast<unsigned>(StateList::SIZE), "STATE_COUNT != StateList::SIZE");

	using Core					= CoreT<Args>;

	using Apex					= MaterialT<0, Args, TApex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	/// @brief Transition
	using Transition			= typename Core::Transition;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(11)	explicit R_(Context& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(11)	explicit R_(PureContext&& context
								  FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(NO) R_(const R_& )														  noexcept = default;
	FFSM2_CONSTEXPR(NO) R_(		 R_&&)														  noexcept = default;

	FFSM2_CONSTEXPR(20)	~R_()																  noexcept;

	//----------------------------------------------------------------------

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(14)		  Context& context()											  noexcept	{ return _core.context;							}

	/// @brief Access context
	/// @return context
	FFSM2_CONSTEXPR(11)	const Context& context()										const noexcept	{ return _core.context;							}

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()														  noexcept	{ return index<StateList, TState>();			}

	//----------------------------------------------------------------------

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(14)		  TState& access()												  noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	FFSM2_CONSTEXPR(11)	const TState& access()											const noexcept	{ return static_cast<const TState&>(_apex);		}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	FFSM2_CONSTEXPR(14)	void update()														  noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void react(const TEvent& event)										  noexcept;

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_CONSTEXPR(11)	StateID activeStateId()											const noexcept	{ return _core.registry.active;							}

	//----------------------------------------------------------------------

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive(const StateID stateId_)							const noexcept	{ return _core.registry.active == stateId_;				}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive()													const noexcept	{ return _core.registry.active == stateId<TState>();	}

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	FFSM2_CONSTEXPR(14)	void changeTo		 (const StateID stateId_)						  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeTo		 ()												  noexcept	{ changeTo (stateId<TState>());							}

	// COMMON
	//----------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	/// @see FFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer 'SerialBuffer' to serialize to
	/// @see FFSM2_ENABLE_SERIALIZATION
	FFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer 'SerialBuffer' to de-serialize from
	/// @see FFSM2_ENABLE_SERIALIZATION
	FFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)								  noexcept;

#endif

	//----------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the transition recorded during last 'update()' / 'react()'
	/// @return Array of last recorded transitions
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(11)	const Transition& previousTransition()							const noexcept	{ return _core.previousTransition;	}

	/// @brief Force process a transition (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param destination Transition destination
	/// @return Success status
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(14)	bool replayTransition(const StateID destination)					  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

	//----------------------------------------------------------------------

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	FFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)								  noexcept	{ _core.logger = logger;			}

#endif

	//----------------------------------------------------------------------

protected:
	FFSM2_CONSTEXPR(14)	void initialEnter()													  noexcept;
	FFSM2_CONSTEXPR(14)	void finalExit()													  noexcept;

	FFSM2_CONSTEXPR(14)	void processRequest()												  noexcept;
	FFSM2_CONSTEXPR(14)	void processTransitions(Transition& currentTransition)				  noexcept;

	FFSM2_CONSTEXPR(14)	bool applyRequest(const Transition& currentTransition,
										  const StateID destination)						  noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByEntryGuards(const Transition& currentTransition,
													const Transition& pendingTransition)	  noexcept;

	FFSM2_CONSTEXPR(14)	bool cancelledByGuards(const Transition& currentTransition,
											   const Transition& pendingTransition)			  noexcept;

	Core _core;
	Apex _apex;
};

////////////////////////////////////////////////////////////////////////////////

// Automatic / manual [de]activation

template <typename, typename>
class RV_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Automatic enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	FFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))  noexcept;

	FFSM2_CONSTEXPR(14)	RV_(const RV_&  other)												  noexcept;
	FFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)												  noexcept;

	FFSM2_CONSTEXPR(20)	~RV_()																  noexcept;

protected:
	using Base::initialEnter;
	using Base::finalExit;

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_core;
	using Base::_apex;
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Manual enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, Manual, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, Manual, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, Manual, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	using typename Base::Transition;

private:
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
#endif

public:
	using Base::Base;

	/// @brief Check if FSM is active
	/// @return FSM active status
	FFSM2_CONSTEXPR(11)	bool isActive()													const noexcept	{ return _core.registry.isActive();	}

	using Base::isActive;

	/// @brief Manually start the FSM
	///   Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	FFSM2_CONSTEXPR(14)	void enter()														  noexcept	{ initialEnter();	}

	/// @brief Manually stop the FSM
	///   Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	FFSM2_CONSTEXPR(14)	void exit()															  noexcept	{ finalExit();		}

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///   Can be used with UE4 USTRUCT() NetSerialize() to load replicated FSM from FArchive
	/// @param destination Transition destination
	/// @see FFSM2_ENABLE_TRANSITION_HISTORY
	FFSM2_CONSTEXPR(14)	void replayEnter(const StateID destination)							  noexcept;

#endif

protected:
	using Base::initialEnter;
	using Base::finalExit;

	using Base::_core;

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Apex					= typename Base::Apex;

	using Base::applyRequest;

	using Base::_apex;
#endif
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Non-'void' payloads

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Transition			= TransitionT<TPayload>;

public:
	using typename Base::Payload;

public:
	using Base::Base;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()														  noexcept	{ return Base::template stateId<TState>();		}

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith(const StateID stateId_,
										const Payload &payload)								  noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_CONSTEXPR(14)	void changeWith(const StateID stateId_,
											Payload&& payload)								  noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(const Payload& payload)								  noexcept	{ changeWith(stateId<TState>(),		 payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith(Payload&& payload)									  noexcept	{ changeWith(stateId<TState>(), move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif

	//----------------------------------------------------------------------

protected:
	using Base::_core;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename, typename>
class InstanceT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT		<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(Context& context
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(11)	explicit InstanceT(PureContext&& context
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(NO) InstanceT(const InstanceT& )												  noexcept = default;
	FFSM2_CONSTEXPR(NO) InstanceT(		InstanceT&&)												  noexcept = default;

private:
	using Base::_core;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext&

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT		<G_<NFeatureTag, TContext&, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, TContext&, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext&, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	using Base::Base;

private:
	using Base::_core;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext*

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT		<G_<NFeatureTag, TContext*, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RP_<G_<NFeatureTag, TContext*, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext*, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(Context context = nullptr
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	  noexcept;

	FFSM2_CONSTEXPR(NO)	InstanceT(const InstanceT& )												  noexcept = default;
	FFSM2_CONSTEXPR(NO)	InstanceT(		InstanceT&&)												  noexcept = default;

	FFSM2_CONSTEXPR(14)	void setContext(Context context)											  noexcept	{ _core.context = context; }

private:
	using Base::_core;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TActivation
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class FFSM2_EMPTY_BASES InstanceT<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public				  RP_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, EmptyContext
{
	using Base				= RP_<G_<NFeatureTag, EmptyContext, TActivation, NSubstitutionLimit FFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))  noexcept;

	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
