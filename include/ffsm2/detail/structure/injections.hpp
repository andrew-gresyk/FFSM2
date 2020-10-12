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

	using Control		= ControlT<TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#ifdef FFSM2_ENABLE_PLANS
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	FFSM2_INLINE void preEntryGuard(Context&)		{}

	FFSM2_INLINE void preEnter	   (Context&)		{}
	FFSM2_INLINE void preReenter   (Context&)		{}

	FFSM2_INLINE void preUpdate	   (Context&)		{}

	template <typename TEvent>
	FFSM2_INLINE void preReact	   (const TEvent&,
									Context&)		{}

	FFSM2_INLINE void preExitGuard (Context&)		{}

	FFSM2_INLINE void postExit	   (Context&)		{}

	template <typename T>
	static constexpr StateID  stateId()		{ return index<StateList, T>();	}
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct B_<TFirst, TRest...>
	: TFirst
	, B_<TRest...>
{
	using typename TFirst::Context;

	using typename TFirst::StateList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#ifdef FFSM2_ENABLE_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void widePreEntryGuard(Context& context);

	FFSM2_INLINE void widePreEnter	   (Context& context);
	FFSM2_INLINE void widePreReenter   (Context& context);

	FFSM2_INLINE void widePreUpdate	   (Context& context);

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context);

	FFSM2_INLINE void widePreExitGuard (Context& context);

	FFSM2_INLINE void widePostExit	   (Context& context);
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

#ifdef FFSM2_ENABLE_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;

	FFSM2_INLINE void entryGuard	   (GuardControl&)			{}

	FFSM2_INLINE void enter			   (PlanControl&)			{}
	FFSM2_INLINE void reenter		   (PlanControl&)			{}

	FFSM2_INLINE void update		   (FullControl&)			{}

	template <typename TEvent>
	FFSM2_INLINE void react			   (const TEvent&,
					 				    FullControl&)			{}

	FFSM2_INLINE void exitGuard		   (GuardControl&)			{}

	FFSM2_INLINE void exit			   (PlanControl&)			{}

#ifdef FFSM2_ENABLE_PLANS
	FFSM2_INLINE void planSucceeded	   (FullControl& control)	{ control.succeed();	}
	FFSM2_INLINE void planFailed	   (FullControl& control)	{ control.fail();		}
#endif

	FFSM2_INLINE void widePreEntryGuard(Context& context);

	FFSM2_INLINE void widePreEnter	   (Context& context);
	FFSM2_INLINE void widePreReenter   (Context& context);

	FFSM2_INLINE void widePreUpdate	   (Context& context);

	template <typename TEvent>
	FFSM2_INLINE void widePreReact	   (const TEvent& event,
					  				    Context& context);

	FFSM2_INLINE void widePreExitGuard (Context& context);

	FFSM2_INLINE void widePostExit	   (Context& context);
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Dynamic_ {};

template <typename... TI>
struct DB_
	: Dynamic_
	, B_<TI...>
{};

template <typename TArgs>
using DynamicEmptyT = DB_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Static_ {};

template <typename... TI>
struct SB_
	: Static_
	, B_<TI...>
{};

template <typename TArgs>
using StaticEmptyT = SB_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "injections.inl"
