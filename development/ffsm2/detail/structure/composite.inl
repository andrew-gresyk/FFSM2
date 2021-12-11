namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.active != INVALID_SHORT);

	const Short  requested  = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return SubStates::wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = control._registry.requested;
	FFSM2_ASSERT(requested != INVALID_SHORT);

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   == INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	active	  = requested;
	requested = INVALID_SHORT;

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//------------------------------------------------------------------------------
// COMMON

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (HeadState::deepUpdate(control)) {
		ControlLock lock{control};

		SubStates::wideUpdate(control, active);
	} else {
		FFSM2_IF_PLANS(const Status subStatus =)
		SubStates::wideUpdate(control, active);

	#if FFSM2_PLANS_AVAILABLE()
		if (subStatus && control._planData.planExists)
			control.updatePlan((HeadState&) *this, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepReact(FullControl& control,
							 const TEvent& event) noexcept
{
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested == INVALID_SHORT);

	if (HeadState::deepReact(control, event)) {
		ControlLock lock{control};

		SubStates::wideReact(control, event, active);
	} else {
		FFSM2_IF_PLANS(const Status subStatus =)
		SubStates::wideReact(control, event, active);

	#if FFSM2_PLANS_AVAILABLE()
		if (subStatus && control._planData.planExists)
			control.updatePlan((HeadState&) *this, subStatus);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	const Short  active  = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	return SubStates::wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	FFSM2_ASSERT(control._registry.requested != INVALID_SHORT);

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active = control._registry.active;
	FFSM2_ASSERT(active != INVALID_SHORT);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	active = INVALID_SHORT;

#if FFSM2_PLANS_AVAILABLE()
	auto plan = control.plan();
	plan.clear();
#endif
}

// COMMON
//------------------------------------------------------------------------------
// COMMON

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& active	 = control._registry.active;
	Short& requested = control._registry.requested;

	FFSM2_ASSERT(active	   != INVALID_SHORT);
	FFSM2_ASSERT(requested != INVALID_SHORT);

	if (requested != active) {
		SubStates::wideExit	  (control, active);

		active	  = requested;
		requested = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	} else {
		requested = INVALID_SHORT;

		// reconstruction done in S_::reenter()
		SubStates::wideReenter(control, active);
	}
}

//------------------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepSaveActive(const Registry& registry,
								  WriteStream& stream) const noexcept
{
	stream.template write<WIDTH_BITS>(registry.active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepLoadRequested(Registry& registry,
									 ReadStream& stream) const noexcept
{
	registry.requested = stream.template read<WIDTH_BITS>();
	FFSM2_ASSERT(registry.requested < WIDTH);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
