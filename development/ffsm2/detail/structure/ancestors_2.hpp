namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TFirst>
struct A_<TFirst>
	: TFirst
{
	using First		= TFirst;
	using typename First::Context;

	using typename First::StateList;

	using typename First::ConstControl;
	using typename First::Control;
	using typename First::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename First::Plan;
#endif

	using typename First::FullControl;
	using typename First::GuardControl;

	using First::stateId;

#if FFSM2_UTILITY_THEORY_AVAILABLE()
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	query		  (		 TEvent&		,
											   ConstControl&		)	  const noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)			noexcept	{}

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void	planSucceeded ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	planFailed	  ( FullControl&		)			noexcept	{}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)			noexcept;
	FFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	wideQuery	  (		 TEvent& event	,
											   ConstControl& control)	  const noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)			noexcept;

	FFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "ancestors_2.inl"
