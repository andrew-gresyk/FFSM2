namespace ffsm2 {

////////////////////////////////////////////////////////////////////////////////

enum class Method : uint8_t {
	NONE,

	ENTRY_GUARD,
	ENTER,
	REENTER,
	PRE_UPDATE,
	UPDATE,
	POST_UPDATE,
	PRE_REACT,
	REACT,
	QUERY,
	POST_REACT,
	EXIT_GUARD,
	EXIT,

//#if FFSM2_PLANS_AVAILABLE()
	PLAN_SUCCEEDED,
	PLAN_FAILED,
//#endif

	COUNT
};

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

static
FFSM2_CONSTEXPR(14)
const char*
methodName(const Method method)											noexcept	{
	switch (method) {
	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::REENTER:		 return "reenter";
	case Method::PRE_UPDATE:	 return "preUpdate";
	case Method::UPDATE:		 return "update";
	case Method::POST_UPDATE:	 return "postUpdate";
	case Method::PRE_REACT:		 return "preReact";
	case Method::REACT:			 return "react";
	case Method::POST_REACT:	 return "postReact";
	case Method::QUERY:			 return "query";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";

//#if FFSM2_PLANS_AVAILABLE()
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
//#endif

	default:
		FFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct TransitionBase {
	FFSM2_CONSTEXPR(11)
	TransitionBase()													noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	TransitionBase(const StateID destination_)							noexcept
		: destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	TransitionBase(const StateID origin_,
				   const StateID destination_)							noexcept
		: origin	 {origin_}
		, destination{destination_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionBase& other)					  const noexcept	{
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionBase& other)					  const noexcept	{
		return origin	   != other.origin ||
			   destination != other.destination ||
			   method	   != other.method;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	explicit
	operator bool()												  const noexcept	{
		return destination != INVALID_STATE_ID;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	void
	clear()																noexcept	{
		destination	= INVALID_STATE_ID;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	StateID	origin		= INVALID_STATE_ID;
	StateID	destination	= INVALID_STATE_ID;
	Method	method		= Method::NONE;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload>
struct TransitionT final
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using TransitionBase::TransitionBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT()														noexcept	{
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination_,
				const Payload& payload)									noexcept
		: TransitionBase{destination_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin_,
				const StateID destination_,
				const Payload& payload)									noexcept
		: TransitionBase{origin_, destination_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload == other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator != (other) ||
			   (payloadSet != other.payloadSet);
		//	   (payloadSet |= other.payloadSet || payload != other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)
	const Payload*
	payload()													  const noexcept	{
		return payloadSet ?
			reinterpret_cast<const Payload*>(&storage) : nullptr;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Payload) Storage storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	bool payloadSet = false;
};

//------------------------------------------------------------------------------

template <>
struct TransitionT<void> final
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}

struct Request final {
	Short index;
};

////////////////////////////////////////////////////////////////////////////////

}
