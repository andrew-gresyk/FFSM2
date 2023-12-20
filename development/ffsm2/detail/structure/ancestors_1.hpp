namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct A_;

template <typename TArgs>
using EmptyT = A_<B_<TArgs>>;

//------------------------------------------------------------------------------

template <typename TFirst, typename... TRest>
struct FFSM2_EMPTY_BASES A_<TFirst, TRest...>
	: TFirst
	, A_<TRest...>
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

	using Rest		= A_<TRest...>;

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

#include "ancestors_1.inl"
