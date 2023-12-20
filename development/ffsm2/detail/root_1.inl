namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(Context& context
																		FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(PureContext&& context
																		FFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
	FFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(const RV_& other) noexcept
	: Base{other}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(RV_&& other) noexcept
	: Base{move(other)}
{}

//------------------------------------------------------------------------------

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(20)
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::~RV_() noexcept {
	finalExit();
}

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	stream.template write<1>(1);
	save(stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Automatic, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (FFSM2_CHECKED(stream.template read<1>()))
		Base::load(stream);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	if (isActive()) {
		stream.template write<1>(1);

		save(stream);
	}
	else
		stream.template write<1>(0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (stream.template read<1>()) {
		if (isActive())
			Base::load(stream);
		else
			loadEnter (stream);
	}
	else
		if (isActive())
			finalExit();
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::replayEnter(const StateID destination) noexcept {
	FFSM2_ASSERT(_core.registry.active == INVALID_SHORT);
	FFSM2_ASSERT(!_core.request);
	FFSM2_IF_TRANSITION_HISTORY(FFSM2_ASSERT(!_core.previousTransition));

	Transition currentTransition;
	PlanControl control{_core, currentTransition};

	applyRequest(currentTransition,
				 destination);

	FFSM2_IF_TRANSITION_HISTORY(_core.previousTransition = Transition{destination});

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	FFSM2_IF_ASSERT(FFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

#endif

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, Short NSL_ FFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
FFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, NSL_ FFSM2_IF_PLANS(, NTC_), TP_>, TA_>::loadEnter(ReadStream& stream) noexcept {
	FFSM2_ASSERT(_core.registry.empty());
	_apex.deepLoadRequested(_core.registry, stream);

	FFSM2_ASSERT(!_core.request);

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_ASSERT(_core.planData.empty() == 0);
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	FFSM2_ASSERT(_core.transitionTargets  .empty());
	FFSM2_ASSERT(_core.previousTransitions.empty());
#endif

	Transition emptyTransition;
	PlanControl control{_core, emptyTransition};

	_apex.deepEnter(control);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
