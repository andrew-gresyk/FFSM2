namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC>::operator[] (const N index) noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
const T&
StaticArrayT<T, NC>::operator[] (const N index) const noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC>::empty() const noexcept {
	for (const Item& item : _items)
		if (item != filler<Item>())
			return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(const TArgs&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{args...};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(TArgs&&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{forward<TArgs>(args)...};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) noexcept {
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
FFSM2_CONSTEXPR(14)
const typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) const noexcept {
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

//------------------------------------------------------------------------------
// SPECIFIC

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const Item& item) noexcept {
	emplace(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
FFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (Item&& item) noexcept {
	emplace(move(item));

	return *this;
}

// SPECIFIC
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
FFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		emplace(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
