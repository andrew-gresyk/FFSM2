namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename
  , typename
  FFSM2_IF_SERIALIZATION(, Long)
  FFSM2_IF_PLANS(, Long)
  , typename
>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry final {
	FFSM2_CONSTEXPR(11)	bool isActive				()						  const noexcept	{ return active != INVALID_SHORT;								}

	FFSM2_CONSTEXPR(14)	bool isActive				(const StateID stateId)	  const noexcept	{
		return stateId == 0 ?
			active != INVALID_SHORT :
			active == stateId;
	}

	FFSM2_CONSTEXPR(14)	void clearRequests			()								noexcept	{		 requested  = INVALID_SHORT;							}
	FFSM2_CONSTEXPR(14)	void clear					()								noexcept	{		 requested  = INVALID_SHORT;   active  = INVALID_SHORT;	}

	FFSM2_CONSTEXPR(11)	bool empty					()						  const noexcept	{ return requested == INVALID_SHORT && active == INVALID_SHORT;	}

	Short requested	= INVALID_SHORT;
	Short active	= INVALID_SHORT;
};

////////////////////////////////////////////////////////////////////////////////

}
}
