namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	StateID NStateId
  , typename TArgs
  , Prong NProng
  , typename... TStates
>
struct FFSM2_EMPTY_BASES CS_<
							 NStateId
						   , TArgs
						   , NProng
						   , TL_<TStates...>
						 >
	:					 LHalfCS<
							 NStateId
						   , TArgs
						   , NProng
						   , TL_<TStates...>
						 >
	,					 RHalfCS<
							 NStateId
						   , TArgs
						   , NProng
						   , TL_<TStates...>
						 >
{
	static_assert(sizeof...(TStates) >= 2, "");

	static constexpr Prong	  PRONG_INDEX = NProng;

	using Args			= TArgs;
	using SubStateList	= TL_<TStates...>;

	using StateList		= typename Args::StateList;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr StateID  INITIAL_ID  = NStateId;
	static constexpr Prong	  L_PRONG	  = PRONG_INDEX;
	static constexpr Prong	  R_PRONG	  = PRONG_INDEX + sizeof...(TStates) / 2;

	using LHalf			= LHalfCS<
							  INITIAL_ID
							, Args
							, PRONG_INDEX
							, SubStateList
						  >;

	using RHalf			= RHalfCS<
							  INITIAL_ID
							, Args
							, PRONG_INDEX
							, SubStateList
						  >;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool		wideEntryGuard		 (GuardControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideEnter			 ( PlanControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	void		wideReenter			 ( PlanControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	TaskStatus	widePreUpdate		 ( FullControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	wideUpdate			 ( FullControl& control,					  const Prong prong)		noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	widePostUpdate		 ( FullControl& control,					  const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	widePreReact		 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	wideReact			 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	widePostReact		 ( FullControl& control, const TEvent& event, const Prong prong)		noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		wideQuery			 (ConstControl& control,	   TEvent& event, const Prong prong)  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	TaskStatus	wideUpdatePlans		 ( FullControl& control,					  const Prong prong)		noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		wideExitGuard		 (GuardControl& control,					  const Prong prong)		noexcept;

	FFSM2_CONSTEXPR(14)	void		wideExit			 ( PlanControl& control,					  const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void		wideChangeToRequested( PlanControl& control,					  const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
