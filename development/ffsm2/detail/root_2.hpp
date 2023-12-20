namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename
  , typename
>
class RP_;

//------------------------------------------------------------------------------
// Non-'void' payloads

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

	using Transition			= TransitionT<TPayload>;

public:
	using typename Base::Payload;

public:
	using Base::Base;
	using Base::processRequest;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()												noexcept	{ return Base::template stateId<TState>();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	FFSM2_CONSTEXPR(14)	void changeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void changeWith				(const Payload& payload)		noexcept	{ changeWith(stateId<TState>(),		 payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `changeWith()`
	FFSM2_CONSTEXPR(14)	void immediateChangeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `changeWith()`
	template <typename TState>
	FFSM2_CONSTEXPR(14)	void immediateChangeWith	(const Payload& payload)		noexcept	{ immediateChangeWith   (stateId<TState>(), payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Base::_core;
};

//------------------------------------------------------------------------------

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TApex
>
class FFSM2_EMPTY_BASES RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>
	: public			RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>
{
	using Base =		RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>;

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_2.inl"
