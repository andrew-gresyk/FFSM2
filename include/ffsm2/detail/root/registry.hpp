namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename
		, typename
		, typename
		, Long
		FFSM2_IF_PLANS(, Long)
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry {
	constexpr void clearRequests() noexcept {
		requested = INVALID_SHORT;
	}

	Short active	= INVALID_SHORT;
	Short requested	= INVALID_SHORT;
};

////////////////////////////////////////////////////////////////////////////////

}
}
