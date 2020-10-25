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

#ifdef FFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr Short	  L_PRONG	  = PRONG_INDEX;

	using LStateList	= LHalf<TStates...>;
	using LMaterial		= CSubMaterial<INITIAL_ID,
									   Args,
									   L_PRONG,
									   LStateList>;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using RStateList	= RHalf<TStates...>;
	using RMaterial		= CSubMaterial<INITIAL_ID + LStateList::SIZE,
									   Args,
									   R_PRONG,
									   RStateList>;

	//----------------------------------------------------------------------

	constexpr bool	 wideEntryGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideConstruct		  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideEnter			  (PlanControl&  control, const Short prong) noexcept;
	constexpr void	 wideReenter		  (PlanControl&  control, const Short prong) noexcept;

	constexpr Status wideUpdate			  (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	constexpr Status wideReact			  (FullControl&  control,
										   const TEvent& event,	  const Short prong) noexcept;

	constexpr bool	 wideExitGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideExit			  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideDestruct		  (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void	 wideChangeToRequested(PlanControl&  control, const Short prong) noexcept;

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

	constexpr bool	 wideEntryGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideConstruct		  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideEnter			  (PlanControl&  control, const Short prong) noexcept;
	constexpr void	 wideReenter		  (PlanControl&  control, const Short prong) noexcept;

	constexpr Status wideUpdate			  (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	constexpr Status wideReact			  (FullControl&  control,
										   const TEvent& event,	  const Short prong) noexcept;

	constexpr bool	 wideExitGuard		  (GuardControl& control, const Short prong) noexcept;

	constexpr void	 wideExit			  (PlanControl&  control, const Short prong) noexcept;

	constexpr void	 wideDestruct		  (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void	 wideChangeToRequested(PlanControl&  control, const Short prong) noexcept;

	//----------------------------------------------------------------------

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
