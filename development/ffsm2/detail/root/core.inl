namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(Context& context_
				  FFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{context_}
	FFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(PureContext&& context_
				  FFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{move(context_)}
	FFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(const CoreT& other) noexcept
	: context {other.context }
	, registry{other.registry}
#if FFSM2_PLANS_AVAILABLE()
	, planData{other.planData}
#endif
	, request {other.request }
#if FFSM2_LOG_INTERFACE_AVAILABLE()
	, logger  {other.logger  }
#endif
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(CoreT&& other) noexcept
	: context {move(other.context )}
	, registry{move(other.registry)}
#if FFSM2_PLANS_AVAILABLE()
	, planData{move(other.planData)}
#endif
	, request {move(other.request )}
#if FFSM2_LOG_INTERFACE_AVAILABLE()
	, logger  {move(other.logger  )}
#endif
{}

////////////////////////////////////////////////////////////////////////////////

}
}
