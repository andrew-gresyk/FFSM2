namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename
		, typename
		, typename
		, Long
		FFSM2_IF_DYNAMIC_PLANS(, Long)
		FFSM2_IF_STATIC_PLANS(, typename)
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry {
	FFSM2_INLINE void clearRequests() noexcept {
		requested = INVALID_SHORT;
	}

	Short active	= INVALID_SHORT;
	Short requested	= INVALID_SHORT;
};

////////////////////////////////////////////////////////////////////////////////

}
}
