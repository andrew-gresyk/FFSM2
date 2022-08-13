namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
struct CoreT {
	using Context			= typename TArgs::Context;
	using PureContext		= typename TArgs::PureContext;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	Transition previousTransition;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	explicit CoreT(Context& context_
									 FFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	FFSM2_CONSTEXPR(11)	explicit CoreT(PureContext&& context_
									 FFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	FFSM2_CONSTEXPR(11) CoreT(const CoreT&  other)													noexcept;
	FFSM2_CONSTEXPR(11) CoreT(		CoreT&& other)													noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	Context context;
	Registry registry;
	FFSM2_IF_PLANS(PlanData planData);
	Transition request;
	FFSM2_IF_LOG_INTERFACE(Logger* logger);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "core.inl"
