namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
StaticArrayT<T, NC>::StaticArrayT(const Item filler) noexcept {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
StaticArrayT<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
StaticArrayT<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Long i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
Long
ArrayT<T, NC>::append(const TValue& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
Long
ArrayT<T, NC>::append(TValue&& value) noexcept {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
ArrayT<T, NC>::operator[] (const N i) noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
ArrayT<T, NC>::operator[] (const N i) const noexcept {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const Item& item) noexcept {
	append(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (Item&& item) noexcept {
	append(std::move(item));

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
