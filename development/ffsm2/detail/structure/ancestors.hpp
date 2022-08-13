namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class B_ {
	template <typename...>
	friend struct A_;

protected:
	using Context		= typename TArgs::Context;

	using StateList		= typename TArgs::StateList;

	using ConstControl	= ConstControlT<TArgs>;
	using Control		= ControlT	   <TArgs>;
	using PlanControl	= PlanControlT <TArgs>;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	static constexpr StateID stateId()											noexcept	{ return index<StateList, T>();				}
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct A_;

//------------------------------------------------------------------------------

template <typename TFirst, typename... TRest>
struct FFSM2_EMPTY_BASES A_<TFirst, TRest...>
	: TFirst
	, A_<TRest...>
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::ConstControl;
	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

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

//------------------------------------------------------------------------------

template <typename TFirst>
struct A_<TFirst>
	: TFirst
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::ConstControl;
	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

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

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = A_<B_<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "ancestors_1.inl"
#include "ancestors_2.inl"
