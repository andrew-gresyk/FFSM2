namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class R_ {
	using Context				= typename TConfig::Context;

	using Payload				= typename TConfig::Payload;

	using Apex					= TApex;

	using Forward				= RF_<TConfig, Apex>;
	using StateList				= typename Forward::StateList;
	using Args					= typename Forward::Args;

	using MaterialApex			= Material<I_<0>, Args, Apex>;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

public:
	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the hierarchy. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSets		= typename Control::TransitionSets;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	explicit R_(Context& context
				FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr));

	~R_();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()					{ return			StateList ::template index<TState>();	}

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update();

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	FFSM2_INLINE void react(const TEvent& event);

	//----------------------------------------------------------------------

	/// @brief Get current active state ID
	/// @return Current active state ID
	FFSM2_INLINE StateID activeStateId() const						{ return _registry.active;						}

	//------------------------------------------------------------------------------

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	FFSM2_INLINE void changeTo(const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	FFSM2_INLINE void changeTo()									{ changeTo (stateId<TState>());					}

	//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'ffsm2::LoggerInterfaceT<>' interface
	/// @see FFSM2_ENABLE_LOG_INTERFACE
	void attachLogger(Logger* const logger)							{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter();
	void processTransitions(TransitionSets& currentTransitions);

	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
								const Transition& pendingTransition);

	bool cancelledByGuards(const TransitionSets& currentTransitions,
						   const Transition& pendingTransition);

protected:
	Context& _context;

	Registry _registry;
	Transition _request;

	MaterialApex _apex;

	FFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext
		, Long NSubstitutionLimit
		, typename TPayload
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit, TPayload>, TApex>;

	using Payload				= TPayload;
	using Transition			= TransitionT<Payload>;

public:
	using Base::Base;

	using Base::stateId;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()								{ return Base::template stateId<TState>();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	FFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith   (const Payload& payload)	{ changeWith   (stateId<TState>(		  payload ));	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	FFSM2_INLINE void changeWith   (	 Payload&& payload)	{ changeWith   (stateId<TState>(std::move(payload)));	}

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
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext, NSubstitutionLimit, void>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, NSubstitutionLimit, void>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, NSubstitutionLimit, void>, TApex>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class RW_ final
	: public	 RP_<TConfig, TApex>
{
	using Base = RP_<TConfig, TApex>;

public:
	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == ::ffsm2::EmptyContext

template <
		  Long NSubstitutionLimit,
		  typename TPayload,
		  typename TApex>
class RW_		<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit, TPayload>, TApex> final
	: public RP_<::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit, TPayload>, TApex>
	, ::ffsm2::EmptyContext
{
	using Cfg =  ::ffsm2::ConfigT<::ffsm2::EmptyContext, NSubstitutionLimit, TPayload>;

	using Context	= typename Cfg::Context;
	using Base		= RP_<Cfg, TApex>;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:
	explicit FFSM2_INLINE RW_(FFSM2_IF_LOG_INTERFACE(  Logger* const logger = nullptr))
		: Base{static_cast<Context&>(*this)
			 FFSM2_IF_LOG_INTERFACE(, logger)}
	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
