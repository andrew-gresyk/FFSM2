namespace ffsm2 {

//------------------------------------------------------------------------------

struct EmptyPayload final {};

struct Automatic;
struct Manual;

//------------------------------------------------------------------------------

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using Prong		 = Short;
static constexpr Prong		INVALID_PRONG		= INVALID_SHORT;

using Long		 = uint8_t;
static constexpr Long		INVALID_LONG		= UINT8_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;
static constexpr StateID	ROOT_ID				= 0;

////////////////////////////////////////////////////////////////////////////////

template <
	bool B
  , typename TT
  , typename TF
>
struct ConditionalT final {
	using Type = TT;
};

template <
	typename TT
  , typename TF
>
struct ConditionalT<false, TT, TF> final {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
	bool B
  , typename TT
  , typename TF
>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct RemoveConstT final {
	using Type = T;
};

template <typename T>
struct RemoveConstT<const T> final {
	using Type = T;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using RemoveConst = typename RemoveConstT<T>::Type;

//------------------------------------------------------------------------------

template <typename T>
struct RemoveReferenceT final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&> final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&&> final {
	using Type = T;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using RemoveReference = typename RemoveReferenceT<T>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using Undecorate = RemoveConst<RemoveReference<T>>;

//------------------------------------------------------------------------------

template <typename T>
struct IsValueReferenceT final {
	static const bool VALUE = false;
};

template <typename T>
struct IsValueReferenceT<T&> final {
	static const bool VALUE = true;
};

////////////////////////////////////////////////////////////////////////////////

template <uint64_t N>
using UIndex = Conditional<N <= (1ull <<  8),  uint8_t,
			   Conditional<N <= (1ull << 16), uint16_t,
			   Conditional<N <= (1ull << 32), uint32_t,
												  void>>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <uint64_t N>
using UCapacity = Conditional<N < (1ull <<  8),  uint8_t,
				  Conditional<N < (1ull << 16), uint16_t,
				  Conditional<N < (1ull << 32), uint32_t,
													void>>>;

////////////////////////////////////////////////////////////////////////////////

FFSM2_CONSTEXPR(11)
uint32_t
bitWidth(const uint32_t v)												noexcept	{
	return	v		== 0 ?  0 :
			v >>  1 == 0 ?  1 :
			v >>  2 == 0 ?  2 :
			v >>  3 == 0 ?  3 :
			v >>  4 == 0 ?  4 :
			v >>  5 == 0 ?  5 :
			v >>  6 == 0 ?  6 :
			v >>  7 == 0 ?  7 :

			v >>  8 == 0 ?  8 :
			v >>  9 == 0 ?  9 :
			v >> 10 == 0 ? 10 :
			v >> 11 == 0 ? 11 :
			v >> 12 == 0 ? 12 :
			v >> 13 == 0 ? 13 :
			v >> 14 == 0 ? 14 :
			v >> 15 == 0 ? 15 :

			v >> 16 == 0 ? 16 :
			v >> 17 == 0 ? 17 :
			v >> 18 == 0 ? 18 :
			v >> 19 == 0 ? 19 :
			v >> 20 == 0 ? 20 :
			v >> 21 == 0 ? 21 :
			v >> 22 == 0 ? 22 :
			v >> 23 == 0 ? 23 :

			v >> 24 == 0 ? 24 :
			v >> 25 == 0 ? 25 :
			v >> 26 == 0 ? 26 :
			v >> 27 == 0 ? 27 :
			v >> 28 == 0 ? 28 :
			v >> 29 == 0 ? 29 :
			v >> 30 == 0 ? 30 :
			v >> 31 == 0 ? 31 :

						   32 ;
}

//------------------------------------------------------------------------------

template <uint64_t N>
using UBitWidth = Conditional<N <=  8,  uint8_t,
				  Conditional<N <= 16, uint16_t,
				  Conditional<N <= 32, uint32_t,
										   void>>>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>& t)											noexcept	{
	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>&& t)											noexcept	{
	static_assert(!IsValueReferenceT<T>::VALUE, "");

	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
FFSM2_CONSTEXPR(11)
RemoveReference<T>&&
move(T&& t)																noexcept	{
	return static_cast<RemoveReference<T>&&>(t);
}

//------------------------------------------------------------------------------

template <
	typename TIn,
	typename TOut = TIn
>
FFSM2_CONSTEXPR(11)
const TOut&
constant(TIn& t)														noexcept	{
	return static_cast<const TOut&>(t);
}

////////////////////////////////////////////////////////////////////////////////

template <
	typename T0
  , typename T1
>
FFSM2_CONSTEXPR(11)
T0
min(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 < static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
	typename T0
  , typename T1
>
FFSM2_CONSTEXPR(11)
T0
max(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 > static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

//------------------------------------------------------------------------------

template <
	typename TIndex
  , typename TElement
  , TIndex NCount
>
FFSM2_CONSTEXPR(11)
TIndex
count(const TElement (&)[NCount])										noexcept	{
	return NCount;
}

//------------------------------------------------------------------------------

template <
	typename N1
  , typename N2
>
FFSM2_CONSTEXPR(11)
N1
ceilingDivide(const N1 n,
			  const N2 to)												noexcept
{
	return (n + static_cast<N1>(to) - 1) / static_cast<N1>(to);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_CONSTEXPR(14)
void
fill(T& a, const char value)											noexcept	{
	memset(&a, static_cast<int>(value), sizeof(a));
}

//------------------------------------------------------------------------------

template <typename T>
FFSM2_CONSTEXPR(14)
void
swap(T& l, T& r)														noexcept	{
	T t = ::ffsm2::move(l);
	l   = ::ffsm2::move(r);
	r   = ::ffsm2::move(t);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template <
	typename TO
  , typename TI
>
FFSM2_CONSTEXPR(17)
TO*
reinterpret_launder(      TI* const in)									noexcept	{
#if FFSM2_CONSTEXPR_AVAILABLE(17)
	return ::std::launder(reinterpret_cast<      TO*>(in));
#else
	return                reinterpret_cast<      TO*>(in);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
	typename TO
  , typename TI
>
FFSM2_CONSTEXPR(17)
const TO*
reinterpret_launder(const TI* const in)									noexcept	{
#if FFSM2_CONSTEXPR_AVAILABLE(17)
	return ::std::launder(reinterpret_cast<const TO*>(in));
#else
	return                reinterpret_cast<const TO*>(in);
#endif
}

//------------------------------------------------------------------------------

template<typename T>
FFSM2_CONSTEXPR(14)
void
destroy(T& t)															noexcept	{
	t.~T();
}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConstT;

template <typename>
struct StaticPrintTypeT;

////////////////////////////////////////////////////////////////////////////////

}
