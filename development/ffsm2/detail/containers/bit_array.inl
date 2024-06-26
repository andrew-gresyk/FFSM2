#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
// COMMON

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::set() noexcept {
	for (uint8_t& unit : _storage)
		unit = UINT8_MAX;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NC_>::empty() const noexcept {
	for (const uint8_t& unit : _storage)
		if (unit != uint8_t{0})
			return false;

	return true;
}

//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//------------------------------------------------------------------------------

template <unsigned NC_>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NC_>::get(const TIndex index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NC_>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::set(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NC_>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::clear(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NC_>::operator & (const BitArray& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if ((_storage[i] & other._storage[i]) == 0)
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NC_>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NC_>::operator &= (const BitArray& other) noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		_storage[i] &= other._storage[i];
}

//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
////////////////////////////////////////////////////////////////////////////////

}
}

#endif
