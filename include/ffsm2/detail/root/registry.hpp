namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename
		, typename
		, typename
		, Long
		, typename>
struct ArgsT;

//------------------------------------------------------------------------------

struct Registry
{
	void clearRequests() {
		requested = INVALID_SHORT;
	}

	Short requested	= INVALID_SHORT;
	Short active	= INVALID_SHORT;
};

////////////////////////////////////////////////////////////////////////////////

}
}
