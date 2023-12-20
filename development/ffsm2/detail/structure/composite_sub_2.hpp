namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	StateID NStateId
  , typename TArgs
  , Prong NProng
  , typename TState
>
struct CS_<
		   NStateId
		 , TArgs
		 , NProng
		 , TL_<TState>
	   >
	:  MaterialT<
		   NStateId
		 , TArgs
		 , TState
	   >
{
	static constexpr Prong	  PRONG_INDEX = NProng;

	using Args			= TArgs;

	using StateList		= typename Args::StateList;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Single		= MaterialT<
							  NStateId
							, Args
							, TState
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

#include "composite_sub_2.inl"
