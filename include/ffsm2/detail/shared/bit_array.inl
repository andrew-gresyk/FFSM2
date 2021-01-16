#ifdef FFSM2_ENABLE_PLANS

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::clear() noexcept {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
bool
BitArrayT<TI, NC>::get(const Index index) const noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::set(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::clear(const Index index) noexcept {
	FFSM2_ASSERT(index < CAPACITY);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
