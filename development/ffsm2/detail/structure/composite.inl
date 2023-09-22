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
	const Short  requested  = compoRequested(control);
	FFSM2_ASSERT(requested < WIDTH);

	FFSM2_ASSERT(compoActive(control) < WIDTH);

	return SubStates::wideEntryGuard(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short  requested = compoRequested(control);
	FFSM2_ASSERT(requested < WIDTH);

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	Short& requested = compoRequested(control);
	Short& active	 = compoActive	 (control);

	FFSM2_ASSERT(requested < WIDTH);
	FFSM2_ASSERT(active	  == INVALID_SHORT);

	active	  = requested;
	requested = INVALID_SHORT;

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepReenter(PlanControl& /*control*/) noexcept {
}

//------------------------------------------------------------------------------
// COMMON

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepPreUpdate(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPreUpdate(control);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreUpdate(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepUpdate(control);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideUpdate(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepPostUpdate(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostUpdate(control, active);

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPostUpdate(control);
	FFSM2_IF_PLANS(headStatus(control) |= h);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepPreReact(FullControl& control,
								const TEvent& event) noexcept
{
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPreReact(control, event);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreReact(control, event, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepReact(FullControl& control,
							 const TEvent& event) noexcept
{
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepReact(control, event);
	FFSM2_IF_PLANS(headStatus(control) |= h);

	FFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideReact(control, event, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepPostReact(FullControl& control,
								 const TEvent& event) noexcept
{
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control};

	FFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostReact(control, event, active);

	FFSM2_IF_PLANS(const TaskStatus h =)
		HeadState::deepPostReact(control, event);
	FFSM2_IF_PLANS(headStatus(control) |= h);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
template <typename TEvent>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepQuery(ConstControl& control,
							 TEvent& event) const noexcept
{
	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	HeadState::deepQuery(control, event);
	SubStates::wideQuery(control, event, active);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepUpdatePlans(FullControl& control) noexcept {
	FFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	const TaskStatus s =	 subStatus(control) |
		SubStates::wideUpdatePlans(control, active);

	const bool planExists = control._core.planData.planExists;

	if (s && planExists)
		control.updatePlan(static_cast<HeadState&>(*this), s);
}

#endif

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	FFSM2_ASSERT(compoRequested(control) < WIDTH);

	return SubStates::wideExitGuard(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
bool
C_<TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short  active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	FFSM2_ASSERT(compoRequested(control) < WIDTH);

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TA, typename TH, typename... TS>
FFSM2_CONSTEXPR(14)
void
C_<TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active = compoActive(control);
	FFSM2_ASSERT(active < WIDTH);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	active = INVALID_SHORT;

#if FFSM2_PLANS_AVAILABLE()
	Plan plan = control.plan();
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
	Short& requested = compoRequested(control);
	Short& active	 = compoActive(control);

	FFSM2_ASSERT(active	   < WIDTH);

	FFSM2_ASSERT(requested < WIDTH);

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
