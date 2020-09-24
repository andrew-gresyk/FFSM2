namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
Long
Array<T, NC>::append(const TValue& value) {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
Long
Array<T, NC>::append(TValue&& value) {
	FFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
Array<T, NC>::operator[] (const N i) {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
Array<T, NC>::operator[] (const N i) const {
	FFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
Array<T, NC>&
Array<T, NC>::operator += (const Item& item) {
	append(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
Array<T, NC>&
Array<T, NC>::operator += (Item&& item) {
	append(std::move(item));

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
Array<T, NC>&
Array<T, NC>::operator += (const Array<T, N>& other) {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}