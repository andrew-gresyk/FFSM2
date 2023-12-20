namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	FFSM2_ASSERT(stateId_ < StateList::SIZE ||
				 stateId_ == INVALID_STATE_ID); // specific

	control._originId = stateId_;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

}
}
