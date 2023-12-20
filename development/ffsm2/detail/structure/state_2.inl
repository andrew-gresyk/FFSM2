namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
S_<NN_, TA_, EmptyT<TA_>>::deepEntryGuard(GuardControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::entryGuard,
						   Method::ENTRY_GUARD);

	return false;
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepEnter(PlanControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::enter,
						   Method::ENTER);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepReenter(PlanControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::reenter,
						   Method::REENTER);
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPreUpdate(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::preUpdate,
						   Method::PRE_UPDATE);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepUpdate(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::update,
						   Method::UPDATE);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPostUpdate(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::postUpdate,
						   Method::POST_UPDATE);

	return TaskStatus{};
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPreReact(FullControl& FFSM2_IF_LOG_STATE_METHOD(control),
										const TEvent& FFSM2_UNUSED(event)) noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, FullControl&)>(&Empty::preReact),
						   Method::PRE_REACT);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepReact(FullControl& FFSM2_IF_LOG_STATE_METHOD(control),
									 const TEvent& FFSM2_UNUSED(event)) noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, FullControl&)>(&Empty::react),
						   Method::REACT);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepPostReact(FullControl& FFSM2_IF_LOG_STATE_METHOD(control),
										 const TEvent& FFSM2_UNUSED(event)) noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, FullControl&)>(&Empty::postReact),
						   Method::POST_REACT);

	return TaskStatus{};
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepQuery(ConstControl& FFSM2_IF_LOG_STATE_METHOD(control),
									 TEvent& FFSM2_UNUSED(event)) const noexcept
{
	FFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(TEvent&, ConstControl&) const>(&Empty::query),
						   Method::QUERY);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
TaskStatus
S_<NN_, TA_, EmptyT<TA_>>::deepUpdatePlans(FullControl& control) noexcept {
	if (control._core.planData.tasksFailures .get(STATE_ID))
		return TaskStatus{TaskStatus::FAILURE};
	else
	if (control._core.planData.tasksSuccesses.get(STATE_ID))
		return TaskStatus{TaskStatus::SUCCESS};
	else
		return TaskStatus{};
}

#endif

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
bool
S_<NN_, TA_, EmptyT<TA_>>::deepExitGuard(GuardControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::exitGuard,
						   Method::EXIT_GUARD);

	return false;
}

//------------------------------------------------------------------------------

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::deepExit(PlanControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::exit,
						   Method::EXIT);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::wrapPlanSucceeded(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::planSucceeded,
						   Method::PLAN_SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NN_, typename TA_>
FFSM2_CONSTEXPR(14)
void
S_<NN_, TA_, EmptyT<TA_>>::wrapPlanFailed(FullControl& FFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	FFSM2_LOG_STATE_METHOD(&Empty::planFailed,
						   Method::PLAN_FAILED);

}

#endif

//------------------------------------------------------------------------------

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

//------------------------------------------------------------------------------

#if FFSM2_STRUCTURE_REPORT_AVAILABLE()
#endif

///////////////////////////////////////////////////////////////////////////////

}
}
