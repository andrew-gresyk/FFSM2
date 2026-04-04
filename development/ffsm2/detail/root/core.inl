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
	: context{::ffsm2::move(context_)}
	FFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(const CoreT& other) noexcept
	: context {other.context }
	, registry{other.registry}
	, request {other.request }
	FFSM2_IF_PLANS			   (, planData          {other.planData          })
	FFSM2_IF_TRANSITION_HISTORY(, previousTransition{other.previousTransition})
	FFSM2_IF_LOG_INTERFACE	   (, logger            {other.logger            })
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(CoreT&& other) noexcept
	: context {::ffsm2::move(other.context )}
	, registry{::ffsm2::move(other.registry)}
	, request {::ffsm2::move(other.request )}
	FFSM2_IF_PLANS			   (, planData          {::ffsm2::move(other.planData          )})
	FFSM2_IF_TRANSITION_HISTORY(, previousTransition{::ffsm2::move(other.previousTransition)})
	FFSM2_IF_LOG_INTERFACE	   (, logger            {::ffsm2::move(other.logger            )})
{}

////////////////////////////////////////////////////////////////////////////////

}
}
