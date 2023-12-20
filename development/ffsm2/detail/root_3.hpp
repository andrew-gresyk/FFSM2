namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	typename
  , typename
>
class InstanceT;

//------------------------------------------------------------------------------
// TContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
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
	using Base =		RP_<
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

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(Context& context
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(11)	explicit InstanceT(PureContext&& context
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(NO) InstanceT(const InstanceT& )												noexcept = default;
	FFSM2_CONSTEXPR(NO) InstanceT(		InstanceT&&)												noexcept = default;

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext&

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

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

//------------------------------------------------------------------------------
// TContext*

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(Context context = nullptr
										 FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(NO)	InstanceT(const InstanceT& )												noexcept = default;
	FFSM2_CONSTEXPR(NO)	InstanceT(		InstanceT&&)												noexcept = default;

	FFSM2_CONSTEXPR(14)	void setContext(Context context)											noexcept	{ _core.context = context; }

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext == EmptyContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	FeatureTag NFeatureTag
  , typename TActivation
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RP_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	, public	 EmptyContext
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(11)	explicit InstanceT(FFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept;

	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_3.inl"
