namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC_>::operator[] (const N index) noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
const T&
StaticArrayT<T, NC_>::operator[] (const N index) const noexcept	{
	FFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC_>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC_>::empty() const noexcept {
	for (const Item& item : _items)
		if (item != filler<Item>())
			return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC_>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Index
DynamicArrayT<T, NC_>::emplace(const TArgs&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{args...};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
template <typename... TArgs>
FFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Index
DynamicArrayT<T, NC_>::emplace(TArgs&&... args) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{forward<TArgs>(args)...};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Item&
DynamicArrayT<T, NC_>::operator[] (const N index) noexcept {
	FFSM2_ASSERT(0 <= index && index < _count);

	return _items[static_cast<Index>(index)];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
template <typename N>
FFSM2_CONSTEXPR(14)
const typename DynamicArrayT<T, NC_>::Item&
DynamicArrayT<T, NC_>::operator[] (const N index) const noexcept {
	FFSM2_ASSERT(0 <= index && index < _count);

	return _items[static_cast<Index>(index)];
}

//------------------------------------------------------------------------------
// SPECIFIC

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (const Item& item) noexcept {
	emplace(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
FFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (Item&& item) noexcept {
	emplace(move(item));

	return *this;
}

// SPECIFIC
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
template <Long N>
FFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (const DynamicArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		emplace(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
