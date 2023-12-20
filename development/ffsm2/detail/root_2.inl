namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::changeWith(const StateID  stateId_,
																		   const Payload& payload) noexcept
{
	FFSM2_ASSERT(_core.registry.isActive());

	_core.request = Transition{stateId_, payload};

	FFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, stateId_);
}

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateChangeWith(const StateID  stateId_,
																					const Payload& payload) noexcept
{
	changeWith(stateId_, payload);

	processRequest();
}

////////////////////////////////////////////////////////////////////////////////

}
}
