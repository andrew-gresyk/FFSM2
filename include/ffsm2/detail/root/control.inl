namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId)
	: control{control_}
	, prevId{control._originId}
{
	control._originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Origin::~Origin() {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_)
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) {
	if (!_locked) {
		_request = Transition{_originId, stateId};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, Long NSL, typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL, TTP>>::changeWith(const StateID stateId,
													   const Payload& payload)
{
	if (!_locked) {
		_request = Transition{_originId, stateId, payload};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, Long NSL, typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, NSL, TTP>>::changeWith(const StateID stateId,
													   Payload&& payload)
{
	if (!_locked) {
		_request = Transition{_originId, stateId, std::move(payload)};

		FFSM2_LOG_TRANSITION(context(), _originId, stateId);
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
GuardControlT<TArgs>::cancelPendingTransition() {
	_cancelled = true;

	FFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
