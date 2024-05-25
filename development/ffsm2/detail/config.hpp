namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct G_ final {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;
	using Activation		 = TActivation;

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long TASK_CAPACITY		  = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using Task				 = TaskT<Payload>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context>;
#endif

	/// @brief Set Context type
	/// @tparam `T` Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , Activation, SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Select manual activation strategy
	using ManualActivation	 = G_<FEATURE_TAG, Context, Manual	  , SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	/// @brief Set Substitution limit
	/// @tparam `N` Maximum number times 'guard()' methods can substitute their states for others
	template <Short N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, Activation, N				   FFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Set Task capacity
	/// @tparam `N` Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context, Activation, SUBSTITUTION_LIMIT				  , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam `T` Payload type
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, Activation, SUBSTITUTION_LIMIT FFSM2_IF_PLANS(, TASK_CAPACITY), T      >;
};

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct M_;

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct M_<
		   G_<
			   NFeatureTag
			 , TContext
			 , TActivation
			 , NSubstitutionLimit
			 FFSM2_IF_PLANS(, NTaskCapacity)
			 , TPayload
		   >
	   > final
{
	using Cfg = G_<
					NFeatureTag
				  , TContext
				  , TActivation
				  , NSubstitutionLimit
				  FFSM2_IF_PLANS(, NTaskCapacity)
				  , TPayload
				>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Root
	/// @tparam `THead` Head state
	/// @tparam `TSubStates` Sub-states
	template <typename THead, typename... TSubStates>
	using Root				= RF_<Cfg, CI_<THead, TSubStates...>>;

	/// @brief Headless root
	/// @tparam `TSubStates` Sub-states
	template <				  typename... TSubStates>
	using PeerRoot			= RF_<Cfg, CI_<void,  TSubStates...>>;

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif
};

////////////////////////////////////////////////////////////////////////////////

}

/// @brief Type configuration for MachineT<>
using Config = detail::G_<
						   FFSM2_FEATURE_TAG
						 , EmptyContext
						 , Automatic
						 , 4
						 FFSM2_IF_PLANS(, INVALID_LONG)
						 , void
					   >;

/// @brief 'Template namespace' for FSM classes
/// @tparam `TConfig` `ConfigT<>` type configuration for MachineT<>
/// @see `ConfigT<>`
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

////////////////////////////////////////////////////////////////////////////////

}
