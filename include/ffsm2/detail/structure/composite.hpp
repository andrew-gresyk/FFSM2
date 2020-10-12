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

	FFSM2_INLINE bool deepForwardEntryGuard(GuardControl& control);
	FFSM2_INLINE bool deepEntryGuard	   (GuardControl& control);

	FFSM2_INLINE void deepConstruct		   (PlanControl&  control);

	FFSM2_INLINE void deepEnter			   (PlanControl&  control);

	FFSM2_INLINE void deepUpdate		   (FullControl&  control);

	template <typename TEvent>
	FFSM2_INLINE void deepReact			   (FullControl&  control, const TEvent& event);

	FFSM2_INLINE bool deepForwardExitGuard (GuardControl& control);
	FFSM2_INLINE bool deepExitGuard		   (GuardControl& control);

	FFSM2_INLINE void deepExit			   (PlanControl&  control);

	FFSM2_INLINE void deepDestruct		   (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void deepChangeToRequested(PlanControl&  control);

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
