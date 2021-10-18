namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename... TStates>
struct CS_ final {
	static_assert(sizeof...(TStates) >= 2, "");

	static constexpr StateID  INITIAL_ID  = NStateId;

	static constexpr Short	  PRONG_INDEX = NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr Short	  L_PRONG	  = PRONG_INDEX;

	using LStateList	= LHalfTypes<TStates...>;
	using LMaterial		= CSubMaterial<INITIAL_ID,
									   Args,
									   L_PRONG,
									   LStateList>;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using RStateList	= RHalfTypes<TStates...>;
	using RMaterial		= CSubMaterial<INITIAL_ID + LStateList::SIZE,
									   Args,
									   R_PRONG,
									   RStateList>;

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter			 (PlanControl&  control, const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter			 (PlanControl&  control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	wideUpdate			 (FullControl&  control, const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	wideReact			 (FullControl&  control,
													  const TEvent& event,	 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit			 (PlanControl&  control, const Short prong)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideChangeToRequested(PlanControl&  control, const Short prong)  noexcept;

	//----------------------------------------------------------------------

	LMaterial lHalf;
	RMaterial rHalf;
};

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename TState>
struct CS_<NStateId, TArgs, NIndex, TState> final {
	static constexpr StateID INITIAL_ID	= NStateId;

	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<INITIAL_ID, Args, TState>;

	//----------------------------------------------------------------------

	FFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter			 (PlanControl&  control, const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter			 (PlanControl&  control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	wideUpdate			 (FullControl&  control, const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	wideReact			 (FullControl&  control,
													  const TEvent& event,	 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control, const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit			 (PlanControl&  control, const Short prong)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideChangeToRequested(PlanControl&  control, const Short prong)  noexcept;

	//----------------------------------------------------------------------

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
