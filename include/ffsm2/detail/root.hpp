namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class R_ {
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

	using Payload				= typename TConfig::Payload;

	using Forward				= RF_<TConfig, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;
	using StateList				= typename Forward::StateList;
	using Args					= typename Forward::Args;

	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	using MaterialApex			= Material<0, Args, TApex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using PlanData				= PlanDataT<Args>;

	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

public:
	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSets		= typename Control::TransitionSets;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	FFSM2_INLINE explicit R_(Context& context
						   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	~R_() noexcept;

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()						noexcept	{ return index<StateList, TState>();			}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update() noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_INLINE void react(const TEvent& event)			noexcept;

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_INLINE StateID activeStateId()			  const	noexcept	{ return _registry.active;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	FFSM2_INLINE bool isActive(const StateID stateId) const	noexcept	{ return _registry.active == stateId;			}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	FFSM2_INLINE bool isActive()					  const	noexcept	{ return _registry.active == stateId<TState>();	}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	FFSM2_INLINE void changeTo(const StateID stateId)		noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_INLINE void changeTo()							noexcept	{ changeTo (stateId<TState>());					}

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	inline void attachLogger(Logger* const logger)			noexcept	{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter() noexcept;

	void processTransitions(TransitionSets& currentTransitions) noexcept;

	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
								const Transition& pendingTransition) noexcept;

	bool cancelledByGuards(const TransitionSets& currentTransitions,
						   const Transition& pendingTransition) noexcept;

protected:
	Context& _context;

	Registry _registry;
	FFSM2_IF_DYNAMIC_PLANS(PlanData _planData);

	Transition _request;

	MaterialApex _apex;

	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

	using Payload				= TPayload;
	using Transition			= TransitionT<Payload>;

public:
	using Base::Base;

	using Base::stateId;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()					 noexcept	{ return Base::template stateId<TState>();				}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
								 const Payload& payload) noexcept;

	/// @brief Transition into a state
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith(const StateID  stateId,
									  Payload&& payload) noexcept;

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(const Payload& payload) noexcept	{ changeWith(stateId<TState>(),			  payload );	}

	/// @brief Transition into a state
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith(	  Payload&& payload) noexcept	{ changeWith(stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using Base::_context;
	using Base::_request;

	FFSM2_IF_LOG_INTERFACE(using Base::_logger);
};

//------------------------------------------------------------------------------

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), void>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), void>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), void>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RW_ final
	: public	 RP_<TConfig, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
{
	using Base = RP_<TConfig, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

public:
	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == ::ffsm2::EmptyContext

template <
		  Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
class RW_		<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)> final
	: public RP_<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>
	, ::ffsm2::EmptyContext
{
	using Cfg =  ::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit FFSM2_IF_DYNAMIC_PLANS(, NTaskCapacity), TPayload>;

	using Context	= typename Cfg::Context;
	using Base		= RP_<Cfg, TApex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:
	explicit FFSM2_INLINE RW_(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<Context&>(*this)
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
