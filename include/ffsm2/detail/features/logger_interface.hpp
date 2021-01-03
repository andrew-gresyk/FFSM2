namespace ffsm2 {

#ifdef FFSM2_ENABLE_LOG_INTERFACE

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
		, typename TContext = EmptyContext>
struct LoggerInterfaceT {
	using Context		 = TContext;

	using Method		 = ::ffsm2::Method;
	using StateID		 = ::ffsm2::StateID;

#ifdef FFSM2_ENABLE_ANY_PLANS
	using StatusEvent	 = ::ffsm2::StatusEvent;
#endif

	virtual void recordMethod(Context& /*context*/,
							  const StateID /*origin*/,
							  const Method /*method*/) noexcept
	{}

	virtual void recordTransition(Context& /*context*/,
								  const StateID /*origin*/,
								  const StateID /*target*/) noexcept
	{}

#ifdef FFSM2_ENABLE_ANY_PLANS

	virtual void recordTaskStatus(Context& /*context*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/) noexcept
	{}

	virtual void recordPlanStatus(Context& /*context*/,
								  const StatusEvent /*event*/) noexcept
	{}

#endif

	virtual void recordCancelledPending(Context& /*context*/,
										const StateID /*origin*/) noexcept
	{}

};

////////////////////////////////////////////////////////////////////////////////

#else

template <FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
		, typename TContext = EmptyContext>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}
