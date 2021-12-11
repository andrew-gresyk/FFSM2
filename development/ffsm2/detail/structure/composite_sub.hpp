namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename... TStates>
struct FFSM2_EMPTY_BASES CS_
	: CSubMaterial<NStateId,
				   TArgs,
				   NIndex,
				   LHalfTypes<TStates...>>
	, CSubMaterial<NStateId + LHalfTypes<TStates...>::SIZE,
				   TArgs,
				   NIndex + LHalfTypes<TStates...>::SIZE,
				   RHalfTypes<TStates...>>
{
	static_assert(sizeof...(TStates) >= 2, "");

	using LMaterial		= CSubMaterial<NStateId,
									   TArgs,
									   NIndex,
									   LHalfTypes<TStates...>>;

	using RMaterial		= CSubMaterial<NStateId + LHalfTypes<TStates...>::SIZE,
									   TArgs,
									   NIndex + LHalfTypes<TStates...>::SIZE,
									   RHalfTypes<TStates...>>;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr StateID  INITIAL_ID  = NStateId;
	static constexpr Short	  PRONG_INDEX = NIndex;

	using LStateList	= LHalfTypes<TStates...>;
	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

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
};

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename TState>
struct FFSM2_EMPTY_BASES CS_<NStateId, TArgs, NIndex, TState>
	: Material<NStateId, TArgs, TState>
{
	using State			= Material<NStateId, TArgs, TState>;

	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

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
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
