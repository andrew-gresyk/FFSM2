namespace ffsm2 {

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload final {};

struct Automatic;
struct Manual;

//------------------------------------------------------------------------------

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using Long		 = uint8_t;
static constexpr Long		INVALID_LONG		= UINT8_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

template <bool B,
		  typename TT,
		  typename TF>
struct ConditionalT final {
	using Type = TT;
};

template <typename TT,
		  typename TF>
struct ConditionalT<false, TT, TF> final {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <bool B,
		  typename TT,
		  typename TF>
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
using UCapacity = Conditional<N <= (1ull <<  8) - 1,  uint8_t,
				  Conditional<N <= (1ull << 16) - 1, uint16_t,
				  Conditional<N <= (1ull << 32) - 1, uint32_t,
														 void>>>;

////////////////////////////////////////////////////////////////////////////////

FFSM2_CONSTEXPR(11)
uint32_t
bitWidth(const uint32_t v)							  noexcept {
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
forward(RemoveReference<T>& t)						  noexcept	{
	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
FFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>&& t)						  noexcept	{
	static_assert(!IsValueReferenceT<T>::VALUE, "");

	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
FFSM2_CONSTEXPR(11)
RemoveReference<T>&&
move(T&& t)											  noexcept	{
	return static_cast<RemoveReference<T>&&>(t);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T0,
		  typename T1>
FFSM2_CONSTEXPR(11)
T0
min(const T0 t0,
	const T1 t1)									  noexcept
{
	return t0 < (T0) t1 ?
		   t0 : (T0) t1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T0,
		  typename T1>
FFSM2_CONSTEXPR(11)
T0
max(const T0 t0,
	const T1 t1)									  noexcept
{
	return t0 > (T0) t1 ?
		   t0 : (T0) t1;
}

//------------------------------------------------------------------------------

template <typename TIndex,
		  typename TElement,
		  TIndex NCount>
FFSM2_CONSTEXPR(11)
TIndex
count(const TElement(&)[NCount])					  noexcept	{
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T,
		  typename TT>
FFSM2_CONSTEXPR(11)
T
contain(const T x,
		const TT to)								  noexcept	{ return (x + (T) to - 1) / (T) to;		}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
FFSM2_CONSTEXPR(14)
void
fill(T& a, const char value)						  noexcept { memset(&a, (int) value, sizeof(a));	}

//------------------------------------------------------------------------------

template <typename T>
FFSM2_CONSTEXPR(14)
void
swap(T& l, T& r)									  noexcept	{
	T t = move(l);
	l = move(r);
	r = move(t);
}

//------------------------------------------------------------------------------

template <typename TTo,
		  typename TFrom>
FFSM2_CONSTEXPR(14)
void
overwrite(TTo& to, const TFrom& from)				  noexcept	{
	static_assert(sizeof(TTo) == sizeof(TFrom), "");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TO,
		  typename TI>
FFSM2_CONSTEXPR(14)
TO
convert(const TI& in)								  noexcept	{
	TO out;

	overwrite(out, in);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConstT;

template <typename>
struct StaticPrintTypeT;

////////////////////////////////////////////////////////////////////////////////

}
