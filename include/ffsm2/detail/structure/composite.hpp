namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct C_ final {
	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Head			= THead;
	using HeadState		= S_<INVALID_STATE_ID, Args, Head>;

	using SubStates		= CS_<0, Args, 0, TSubStates...>;

	//----------------------------------------------------------------------

	constexpr bool deepForwardEntryGuard(GuardControl& control) noexcept;
	constexpr bool deepEntryGuard		(GuardControl& control) noexcept;

	constexpr void deepConstruct		(PlanControl&  control) noexcept;

	constexpr void deepEnter			(PlanControl&  control) noexcept;

	constexpr void deepUpdate			(FullControl&  control) noexcept;

	template <typename TEvent>
	constexpr void deepReact			(FullControl&  control,
										 const TEvent& event)	noexcept;

	constexpr bool deepForwardExitGuard	(GuardControl& control) noexcept;
	constexpr bool deepExitGuard		(GuardControl& control) noexcept;

	constexpr void deepExit				(PlanControl&  control) noexcept;

	constexpr void deepDestruct			(PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void deepChangeToRequested(PlanControl&  control) noexcept;

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
