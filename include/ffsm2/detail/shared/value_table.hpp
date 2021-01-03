#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NKey, Long NValue>
struct VP_ {};

//------------------------------------------------------------------------------

template <typename...>
struct VT_;

template <Long... NKeys, Long... NValues>
struct VT_<VP_<NKeys, NValues>...>
	: VP_<NKeys, NValues>...
{};

//------------------------------------------------------------------------------

template <Long NKey, Long NValue>
constexpr Long
matchValue(VP_<NKey, NValue>) noexcept { return NValue; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename TValueTable, Long NKey, typename = void>
struct MatchValueN {
	static constexpr Long VALUE = INVALID_LONG;
};

template<typename TValueTable, Long NKey>
struct MatchValueN<TValueTable, NKey, Void<decltype(matchValue<NKey>(TValueTable{}))>> {
	static constexpr Long VALUE = matchValue<NKey>(TValueTable{});
};

//------------------------------------------------------------------------------

template <typename, typename>
struct MatchValuesN;

template <typename... TKeys, typename... TValues, typename... Ts>
struct MatchValuesN<TT_<TP_<TKeys, TValues>...>, TL_<Ts...>> {
	using TypeList = TL_<Ts...>;

	using ValueTable = VT_<
						   VP_<
							   index<TypeList, TKeys>(),
							   index<TypeList, TValues>()
						   >...
					   >;

	using Type = VL_<
		MatchValueN<
			ValueTable,
			index<TypeList, Ts>()
		>::VALUE...
	>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TTable, typename TList>
using MatchValues = typename MatchValuesN<TTable, TList>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
