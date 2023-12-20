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
	, request {other.request }
	FFSM2_IF_PLANS			   (, planData			 {other.planData		   })
	FFSM2_IF_LOG_INTERFACE	   (, logger			 {other.logger			   })
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
FFSM2_CONSTEXPR(11)
CoreT<TArgs>::CoreT(CoreT&& other) noexcept
	: context {move(other.context )}
	, registry{move(other.registry)}
	, request {move(other.request )}
	FFSM2_IF_PLANS			   (, planData			 {move(other.planData			)})
	FFSM2_IF_LOG_INTERFACE	   (, logger			 {move(other.logger				)})
{}

////////////////////////////////////////////////////////////////////////////////

}
}
