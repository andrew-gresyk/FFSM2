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

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set() noexcept {
	for (uint8_t& unit : _storage)
		unit = UINT8_MAX;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::empty() const noexcept {
	for (const uint8_t& unit : _storage)
		if (unit != uint8_t{0})
			return false;

	return true;
}

//------------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get(const TIndex index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::operator & (const BitArray& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if ((_storage[i] & other._storage[i]) == 0)
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::operator &= (const BitArray& other) noexcept {
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
