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
	constexpr void preEntryGuard(Context&) noexcept {}

	constexpr void preEnter		(Context&) noexcept {}
	constexpr void preReenter	(Context&) noexcept {}

	constexpr void preUpdate	(Context&) noexcept {}

	template <typename TEvent>
	constexpr void preReact		(const TEvent&,
								 Context&) noexcept {}

	constexpr void preExitGuard	(Context&) noexcept {}

	constexpr void postExit		(Context&) noexcept {}

	template <typename T>
	static constexpr StateID  stateId()	   noexcept { return index<StateList, T>();	}
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

	constexpr void widePreEntryGuard(Context& context) noexcept;

	constexpr void widePreEnter		(Context& context) noexcept;
	constexpr void widePreReenter	(Context& context) noexcept;

	constexpr void widePreUpdate	(Context& context) noexcept;

	template <typename TEvent>
	constexpr void widePreReact		(const TEvent& event,
									 Context& context) noexcept;

	constexpr void widePreExitGuard	(Context& context) noexcept;

	constexpr void widePostExit		(Context& context) noexcept;
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

	constexpr void entryGuard		(GuardControl&)	noexcept {}

	constexpr void enter			(PlanControl& )	noexcept {}
	constexpr void reenter			(PlanControl& )	noexcept {}

	constexpr void update			(FullControl& )	noexcept {}

	template <typename TEvent>
	constexpr void react			(const TEvent&,
									 FullControl& )	noexcept {}

	constexpr void exitGuard		(GuardControl&)	noexcept {}

	constexpr void exit				(PlanControl& )	noexcept {}

#ifdef FFSM2_ENABLE_PLANS
	constexpr void planSucceeded	(FullControl& control) noexcept { control.succeed();	}
	constexpr void planFailed		(FullControl& control) noexcept { control.fail();		}
#endif

	constexpr void widePreEntryGuard(Context& context) noexcept;

	constexpr void widePreEnter		(Context& context) noexcept;
	constexpr void widePreReenter	(Context& context) noexcept;

	constexpr void widePreUpdate	(Context& context) noexcept;

	template <typename TEvent>
	constexpr void widePreReact		(const TEvent& event,
									 Context& context) noexcept;

	constexpr void widePreExitGuard	(Context& context) noexcept;

	constexpr void widePostExit		(Context& context) noexcept;
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
