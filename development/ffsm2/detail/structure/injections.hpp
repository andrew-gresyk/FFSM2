namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
	template <typename...>
	friend struct B_;

protected:
	using Context		= typename TArgs::Context;

	using StateList		= typename TArgs::StateList;

	using Control		= ControlT	  <TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	FFSM2_CONSTEXPR(14)	void preEntryGuard	  (Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void preEnter		  (Context&)	  noexcept	{}
	FFSM2_CONSTEXPR(14)	void preReenter		  (Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void preUpdate		  (Context&)	  noexcept	{}
	FFSM2_CONSTEXPR(14)	void postUpdate		  (Context&)	  noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void preReact		  (const TEvent&,
											   Context&)	  noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void postReact		  (const TEvent&,
											   Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void preExitGuard	  (Context&)	  noexcept	{}

	FFSM2_CONSTEXPR(14)	void postExit		  (Context&)	  noexcept	{}

	template <typename T>
	static constexpr StateID stateId()						  noexcept	{ return index<StateList, T>();				}
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct FFSM2_EMPTY_BASES B_<TFirst, TRest...>
	: TFirst
	, B_<TRest...>
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if FFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_CONSTEXPR(14)	void widePreEntryGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreEnter		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePreReenter		  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreUpdate		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePostUpdate		  (Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePreReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePostReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreExitGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePostExit		  (Context& context)	  noexcept;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

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

	FFSM2_CONSTEXPR(14)	void entryGuard			  (GuardControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void enter				  (PlanControl&)		  noexcept	{}
	FFSM2_CONSTEXPR(14)	void reenter			  (PlanControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void update				  (FullControl&)		  noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void react				  (const TEvent&,
												   FullControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void exitGuard			  (GuardControl&)		  noexcept	{}

	FFSM2_CONSTEXPR(14)	void exit				  (PlanControl&)		  noexcept	{}

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void planSucceeded		  (FullControl&)		  noexcept	{}
	FFSM2_CONSTEXPR(14)	void planFailed			  (FullControl&)		  noexcept	{}
#endif

	FFSM2_CONSTEXPR(14)	void widePreEntryGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreEnter		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePreReenter		  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreUpdate		  (Context& context)	  noexcept;
	FFSM2_CONSTEXPR(14)	void widePostUpdate		  (Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePreReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void widePostReact		  (const TEvent& event,
												   Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePreExitGuard	  (Context& context)	  noexcept;

	FFSM2_CONSTEXPR(14)	void widePostExit		  (Context& context)	  noexcept;
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "injections_1.inl"
#include "injections_2.inl"
