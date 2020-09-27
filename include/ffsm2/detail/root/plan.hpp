namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;

	inline Status(const Result result_ = Result::NONE);

	inline explicit operator bool() const	{ return result != Result::NONE; }

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result};
}

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan.inl"
