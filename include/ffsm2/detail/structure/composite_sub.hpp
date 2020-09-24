namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename... TStates>
struct CS_ final {
	static_assert(sizeof...(TStates) >= 2, "");

	using Indices		= TIndices;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;

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

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<I_<INITIAL_ID>,
									   Args,
									   L_PRONG,
									   LStates>;
	using LHalfInfo		= typename InfoT<LHalf>::Type;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT>,
									   Args,
									   R_PRONG,
									   RStates>;

	//----------------------------------------------------------------------

	FFSM2_INLINE bool	 wideEntryGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideConstruct				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideEnter					 (PlanControl&  control,						 const Short prong);
	FFSM2_INLINE void	 wideReenter				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE Status	 wideUpdate					 (FullControl&  control,						 const Short prong);

	template <typename TEvent>
	FFSM2_INLINE Status	 wideReact					 (FullControl&  control, const TEvent& event,	 const Short prong);

	FFSM2_INLINE bool	 wideExitGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideExit					 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideDestruct				 (PlanControl&  control,						 const Short prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	 wideChangeToRequested		 (PlanControl&  control,						 const Short prong);

	//----------------------------------------------------------------------

	LHalf lHalf;
	RHalf rHalf;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TState>
struct CS_<TIndices, TArgs, NIndex, TState> final {
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	= Indices::STATE_ID;

	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<I_<INITIAL_ID>,
								   Args,
								   TState>;

	//----------------------------------------------------------------------

	FFSM2_INLINE bool	 wideEntryGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideConstruct				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideEnter					 (PlanControl&  control,						 const Short prong);
	FFSM2_INLINE void	 wideReenter				 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE Status	 wideUpdate					 (FullControl&  control,						 const Short prong);

	template <typename TEvent>
	FFSM2_INLINE Status	 wideReact					 (FullControl&  control, const TEvent& event,	 const Short prong);

	FFSM2_INLINE bool	 wideExitGuard				 (GuardControl& control,						 const Short prong);

	FFSM2_INLINE void	 wideExit					 (PlanControl&  control,						 const Short prong);

	FFSM2_INLINE void	 wideDestruct				 (PlanControl&  control,						 const Short prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	 wideChangeToRequested		 (PlanControl&  control,						 const Short prong);

	//----------------------------------------------------------------------

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
