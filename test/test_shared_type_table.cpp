#include <type_traits>

#define FFSM2_ENABLE_STATIC_PLANS
#include <ffsm2/machine.hpp>

#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace M = ffsm2::detail;

////////////////////////////////////////////////////////////////////////////////

static constexpr ffsm2::Long INVALID_LONG = ffsm2::INVALID_LONG;

template <typename... Ts>
using TypeList = ffsm2::detail::TL_<Ts...>;

template <typename TKey, typename TValue>
using TypeLink = ffsm2::detail::TP_<TKey, TValue>;

template <typename... Ts>
using TypeTable = ffsm2::detail::TT_<Ts...>;

//------------------------------------------------------------------------------

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

//------------------------------------------------------------------------------

using TT = TypeTable<
	TypeLink<A, B>,
	TypeLink<D, F>,
	TypeLink<B, C>
>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using TB = M::MatchType<TT, A>;
static_assert(std::is_same<TB, B>::value, "");

using TV = M::MatchType<TT, C>;
static_assert(std::is_same<TV, void>::value, "");

//------------------------------------------------------------------------------

using TL = TypeList<A, B, C, D, E, F>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

constexpr auto IB = ffsm2::index<TL, TB>();
static_assert(IB == 1, "");

constexpr auto IV = ffsm2::index<TL, TV>();
static_assert(IV == INVALID_LONG, "");

//------------------------------------------------------------------------------

using TML = M::MatchTypes<TT, TL>;
static_assert(std::is_same<TML, TypeList<B, C, void, F, void, void>>::value, "");

////////////////////////////////////////////////////////////////////////////////

using Long = ffsm2::Long;

template <Long... Ns>
using ValueList = ffsm2::detail::VL_<Ns...>;

template <Long NKey, Long NValue>
using ValueLink = ffsm2::detail::VP_<NKey, NValue>;

template <typename... Ts>
using ValueTable = ffsm2::detail::VT_<Ts...>;

//------------------------------------------------------------------------------

using VT = ValueTable<
	ValueLink<0, 1>,
	ValueLink<3, 5>,
	ValueLink<1, 2>
>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static constexpr Long NB = M::MatchValueN<VT, 1>::VALUE;
static_assert(NB == 2, "");

static constexpr Long NV = M::MatchValueN<VT, 2>::VALUE;
static_assert(NV == INVALID_LONG, "");

//------------------------------------------------------------------------------

using VML = M::MatchValues<TT, TL>;
static_assert(std::is_same<VML, ValueList<1, 2, INVALID_LONG, 5, INVALID_LONG, INVALID_LONG>>::value, "");

////////////////////////////////////////////////////////////////////////////////

using LHalf = M::LHalfValues<0, 1, 2, 3, 4, 5>;
static_assert(std::is_same<LHalf, ValueList<0, 1, 2>>::value, "");

using RHalf = M::RHalfValues<0, 1, 2, 3, 4, 5>;
static_assert(std::is_same<RHalf, ValueList<3, 4, 5>>::value, "");

using VL = ValueList<5, 7, 3, 0, 9, 5>;
using VA = M::BinaryLookup<VL>;

static constexpr Long a0 = VA::get(0);
static constexpr Long a1 = VA::get(1);
static constexpr Long a2 = VA::get(2);
static constexpr Long a3 = VA::get(3);
static constexpr Long a4 = VA::get(4);
static constexpr Long a5 = VA::get(5);

static_assert(a0 == 5, "");
static_assert(a1 == 7, "");
static_assert(a2 == 3, "");
static_assert(a3 == 0, "");
static_assert(a4 == 9, "");
static_assert(a5 == 5, "");

////////////////////////////////////////////////////////////////////////////////

#endif
