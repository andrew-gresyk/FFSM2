#ifdef FFSM2_ENABLE_STATIC_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long... Ns>
struct VL_ {
	static constexpr Long SIZE = sizeof...(Ns);
};

//------------------------------------------------------------------------------

template <Long, typename>
struct PrependN;

template <Long N, Long... Ns>
struct PrependN<N, VL_<Ns...>> {
	using Type = VL_<N, Ns...>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long N, typename VL>
using PrependValues = typename PrependN<N, VL>::Type;

//------------------------------------------------------------------------------

template <Long, Long, Long...>
struct LowerN;

template <Long NHalf, Long NIndex, Long... Ns>
using LowerValues = typename LowerN<NHalf, NIndex, Ns...>::Type;

template <Long NHalf, Long NIndex, Long NFirst, Long... NRest>
struct LowerN<NHalf, NIndex, NFirst, NRest...> {
	using LValueList = typename LowerN<NHalf, NIndex + 1, NRest...>::Type;

	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependValues<NFirst, LValueList>,
					 LValueList
				 >;
};

template <Long NHalf, Long NIndex>
struct LowerN<NHalf, NIndex> {
	using Type = VL_<>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long... Ns>
using LHalfValues = LowerValues<sizeof...(Ns) / 2, 0, Ns...>;

//------------------------------------------------------------------------------

template <Long, Long, Long...>
struct UpperN;

template <Long NHalf, Long NIndex, Long... Ns>
using UpperValues = typename UpperN<NHalf, NIndex, Ns...>::Type;

template <Long NHalf, Long NIndex, Long NFirst, Long... NRest>
struct UpperN<NHalf, NIndex, NFirst, NRest...> {
	using Type = Conditional<
					 (NIndex < NHalf),
					 UpperValues<NHalf, NIndex + 1, NRest...>,
					 VL_<NFirst, NRest...>
				 >;
};

template <Long NHalf, Long NIndex>
struct UpperN<NHalf, NIndex> {
	using Type = VL_<>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long... Ns>
using RHalfValues = UpperValues<sizeof...(Ns) / 2, 0, Ns...>;

//------------------------------------------------------------------------------

template <Long NFirst, typename>
struct BinaryLookupT;

template <Long NFirst, Long... Ns>
struct BinaryLookupT<NFirst, VL_<Ns...>> {
	static constexpr Long FIRST	= NFirst;

	using LHalfList = LHalfValues<Ns...>;
	using RHalfList = RHalfValues<Ns...>;

	using LHalf = BinaryLookupT<FIRST				   , LHalfList>;
	using RHalf = BinaryLookupT<FIRST + LHalfList::SIZE, RHalfList>;

	static constexpr Long get(const long index) {
		return index < FIRST + LHalfList::SIZE ?
			LHalf::get(index) : RHalf::get(index);
	}
};

template <Long NFirst, Long N>
struct BinaryLookupT<NFirst, VL_<N>> {
	static constexpr Long get(const long /*index*/) { return N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TValueList>
using BinaryLookup = BinaryLookupT<0, TValueList>;

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
