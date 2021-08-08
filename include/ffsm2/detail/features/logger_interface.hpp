namespace ffsm2 {

#if FFSM2_LOG_INTERFACE_AVAILABLE()

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = FFSM2_FEATURE_TAG
		, typename TContext = EmptyContext>
struct LoggerInterfaceT {
	using Context		 = TContext;

	using Method		 = ::ffsm2::Method;
	using StateID		 = ::ffsm2::StateID;

#if FFSM2_PLANS_AVAILABLE()
	using StatusEvent	 = ::ffsm2::StatusEvent;
#endif

	FFSM2_CONSTEXPR(NO)
	virtual
	void recordMethod(Context& FFSM2_UNUSED(context),
					  const StateID FFSM2_UNUSED(origin),
					  const Method FFSM2_UNUSED(method))			  noexcept
	{}

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTransition(Context& FFSM2_UNUSED(context),
					 const StateID FFSM2_UNUSED(origin),
					 const StateID FFSM2_UNUSED(target))			  noexcept
	{}

#if FFSM2_PLANS_AVAILABLE()

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTaskStatus(Context& FFSM2_UNUSED(context),
					 const StateID FFSM2_UNUSED(origin),
					 const StatusEvent FFSM2_UNUSED(event))			  noexcept
	{}

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordPlanStatus(Context& FFSM2_UNUSED(context),
					 const StatusEvent FFSM2_UNUSED(event))			  noexcept
	{}

#endif

	FFSM2_CONSTEXPR(NO)
	virtual
	void
	recordCancelledPending(Context& FFSM2_UNUSED(context),
						   const StateID FFSM2_UNUSED(origin))		  noexcept
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
