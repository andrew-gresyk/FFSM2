namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename... TStates>
struct FFSM2_EMPTY_BASES CS_<NStateId,
							 TArgs,
							 NIndex,
							 TL_<TStates...>>
	: LHalfCS<NStateId,
			  TArgs,
			  NIndex,
			  TL_<TStates...>>
	, RHalfCS<NStateId,
			  TArgs,
			  NIndex,
			  TL_<TStates...>>
{
	static_assert(sizeof...(TStates) >= 2, "");

	using Args			= TArgs;
	using SubStateList	= TL_<TStates...>;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr StateID  INITIAL_ID  = NStateId;
	static constexpr Short	  PRONG_INDEX = NIndex;
	static constexpr Short	  R_PRONG	  = PRONG_INDEX + sizeof...(TStates) / 2;

	using LHalf			= LHalfCS<INITIAL_ID,
								  Args,
								  PRONG_INDEX,
								  SubStateList>;

	using RHalf			= RHalfCS<INITIAL_ID,
								  Args,
								  PRONG_INDEX,
								  SubStateList>;

	//----------------------------------------------------------------------
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control,						 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter			 ( PlanControl& control,						 const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter			 ( PlanControl& control,						 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	widePreUpdate		 ( FullControl& control,						 const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	Status	wideUpdate			 ( FullControl& control,						 const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	Status	widePostUpdate		 ( FullControl& control,						 const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	widePreReact		 ( FullControl& control, const TEvent& event,	 const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	wideReact			 ( FullControl& control, const TEvent& event,	 const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	widePostReact		 ( FullControl& control, const TEvent& event,	 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control,						 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit			 ( PlanControl& control,						 const Short prong)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideChangeToRequested( PlanControl& control,						 const Short prong)  noexcept;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  Short NIndex,
		  typename TState>
struct CS_<NStateId,
		   TArgs,
		   NIndex,
		   TL_<TState>>
	: MaterialT<NStateId,
				TArgs,
				TState>
{
	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Single		= MaterialT<NStateId,
									Args,
									TState>;

	//----------------------------------------------------------------------
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control,						 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter			 ( PlanControl& control,						 const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter			 ( PlanControl& control,						 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	Status	widePreUpdate		 ( FullControl& control,						 const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	Status	wideUpdate			 ( FullControl& control,						 const Short prong)  noexcept;
	FFSM2_CONSTEXPR(14)	Status	widePostUpdate		 ( FullControl& control,						 const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	widePreReact		 ( FullControl& control, const TEvent& event,	 const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	wideReact			 ( FullControl& control, const TEvent& event,	 const Short prong)  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	Status	widePostReact		 ( FullControl& control, const TEvent& event,	 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control,						 const Short prong)  noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit			 ( PlanControl& control,						 const Short prong)  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideChangeToRequested( PlanControl& control,						 const Short prong)  noexcept;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
