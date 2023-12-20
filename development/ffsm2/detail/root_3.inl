namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(Context& context
																			  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(PureContext&& context
																			  FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, TC_*, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(Context context
																			   FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(11)
InstanceT<G_<NFT_, EmptyContext, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::InstanceT(FFSM2_IF_LOG_INTERFACE(Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

}
}
