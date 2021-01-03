#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename...>
using Void = void;

//------------------------------------------------------------------------------

template <typename TKey, typename TValue>
struct TP_ {};

//------------------------------------------------------------------------------

template <typename...>
struct TT_;

template <typename... TKeys, typename... TValues>
struct TT_<TP_<TKeys, TValues>...>
	: TP_<TKeys, TValues>...
{};

//------------------------------------------------------------------------------

template <typename TKey, typename TValue>
constexpr TValue
tableValue(TP_<TKey, TValue>) noexcept { return TValue{}; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename TTypeTable, typename TKey, typename = void>
struct MatchT {
	using Type = void;
};

template<typename TTypeTable, typename TKey>
struct MatchT<TTypeTable, TKey, Void<decltype(tableValue<TKey>(TTypeTable{}))>> {
	using Type = decltype(tableValue<TKey>(TTypeTable{}));
};

template<typename TTypeTable, typename TKey>
using MatchType = typename MatchT<TTypeTable, TKey>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename, typename>
struct MatchListT;

template <typename... TKeys, typename... TValues, typename... Ts>
struct MatchListT<TT_<TP_<TKeys, TValues>...>, TL_<Ts...>> {
	using Type = TL_<MatchType<TT_<TP_<TKeys, TValues>...>, Ts>...>;
};

template <typename TTable, typename TList>
using MatchTypes = typename MatchListT<TTable, TList>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
