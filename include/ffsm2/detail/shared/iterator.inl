namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
constexpr bool
Iterator<T>::operator != (const Iterator<T>& FFSM2_IF_ASSERT(dummy)) const noexcept {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
constexpr Iterator<T>&
Iterator<T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
constexpr bool
Iterator<const T>::operator != (const Iterator<const T>& FFSM2_IF_ASSERT(dummy)) const noexcept {
	FFSM2_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
constexpr Iterator<const T>&
Iterator<const T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
