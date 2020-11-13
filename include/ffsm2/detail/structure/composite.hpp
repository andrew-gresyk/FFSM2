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

	FFSM2_INLINE bool deepForwardEntryGuard(GuardControl& control) noexcept;
	FFSM2_INLINE bool deepEntryGuard	   (GuardControl& control) noexcept;

	FFSM2_INLINE void deepConstruct		   (PlanControl&  control) noexcept;

	FFSM2_INLINE void deepEnter			   (PlanControl&  control) noexcept;

	FFSM2_INLINE void deepUpdate		   (FullControl&  control) noexcept;

	template <typename TEvent>
	FFSM2_INLINE void deepReact			   (FullControl&  control,
											const TEvent& event)   noexcept;

	FFSM2_INLINE bool deepForwardExitGuard (GuardControl& control) noexcept;
	FFSM2_INLINE bool deepExitGuard		   (GuardControl& control) noexcept;

	FFSM2_INLINE void deepExit			   (PlanControl&  control) noexcept;

	FFSM2_INLINE void deepDestruct		   (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void deepChangeToRequested(PlanControl&  control) noexcept;

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
