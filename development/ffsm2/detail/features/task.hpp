#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskBase {
	static_assert(sizeof(Long) == sizeof(StateID), "");

	FFSM2_CONSTEXPR(11)	TaskBase()										noexcept	{}

	FFSM2_CONSTEXPR(11)	TaskBase(const StateID origin_,
								 const StateID destination_)			noexcept
		: origin{origin_}
		, destination{destination_}
	{}

	FFSM2_CONSTEXPR(11) bool cyclic()							  const noexcept	{ return origin == destination;	}

	union {
		StateID origin		= INVALID_STATE_ID;
		Long prev;
	};

	union {
		StateID destination	= INVALID_STATE_ID;
		Long next;
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FFSM2_CONSTEXPR(11)
bool
operator == (const TaskBase& lhs,
			 const TaskBase& rhs)										noexcept
{
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination;
}

//------------------------------------------------------------------------------

template <typename TPayload>
struct TaskT final
	: TaskBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];

	using TaskBase::TaskBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	TaskT()											noexcept	{
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	TaskT(const StateID origin_,
							  const StateID destination_,
							  const Payload& payload)					noexcept
		: TaskBase{origin_, destination_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
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
struct TaskT<void> final
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
