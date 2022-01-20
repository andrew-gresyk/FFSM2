﻿namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct FFSM2_EMPTY_BASES C_
	: S_<INVALID_STATE_ID, TArgs, THead>
	, CS_<0, TArgs, 0, TL_<TSubStates...>>
{
	using Args			= TArgs;

	using HeadState		= S_<INVALID_STATE_ID, Args, THead>;
	using SubStates		= CS_<0, Args, 0, TL_<TSubStates...>>;

	using StateList		= typename Args::StateList;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;


#if FFSM2_SERIALIZATION_AVAILABLE()
	using Head			= THead;
	using Info			= CI_<Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(14)	bool deepForwardEntryGuard(GuardControl& control)  noexcept;
	FFSM2_CONSTEXPR(14)	bool deepEntryGuard		  (GuardControl& control)  noexcept;

	FFSM2_CONSTEXPR(14)	void deepEnter			  (PlanControl&  control)  noexcept;
	FFSM2_CONSTEXPR(14)	void deepReenter		  (PlanControl&  control)  noexcept;

	FFSM2_CONSTEXPR(14)	void deepUpdate			  (FullControl&  control)  noexcept;
	FFSM2_CONSTEXPR(14)	void deepReverseUpdate	  (FullControl&  control)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void deepReact			  (FullControl&  control,
												   const TEvent& event)    noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void deepReverseReact	  (FullControl&  control,
												   const TEvent& event)    noexcept;

	FFSM2_CONSTEXPR(14)	bool deepForwardExitGuard (GuardControl& control)  noexcept;
	FFSM2_CONSTEXPR(14)	bool deepExitGuard		  (GuardControl& control)  noexcept;

	FFSM2_CONSTEXPR(14)	void deepExit			  (PlanControl&  control)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void deepChangeToRequested(PlanControl&  control)  noexcept;

	//----------------------------------------------------------------------

#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	FFSM2_CONSTEXPR(14)	void deepSaveActive		  (const Registry& registry, WriteStream& stream) const noexcept;
	FFSM2_CONSTEXPR(14)	void deepLoadRequested	  (		 Registry& registry, ReadStream&  stream) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
