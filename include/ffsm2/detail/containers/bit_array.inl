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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// COMMON

template <unsigned NCapacity>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get(const TIndex index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = (Index) index / 8;
	const Index bit  = (Index) index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = (Index) index / 8;
	const Index bit  = (Index) index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
FFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear(const TIndex index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = (Index) index / 8;
	const Index bit  = (Index) index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
