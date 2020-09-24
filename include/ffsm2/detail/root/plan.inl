namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const Result result_)
	: result{result_}
{}

//------------------------------------------------------------------------------

void
Status::clear() {
	result = Result::NONE;
}

////////////////////////////////////////////////////////////////////////////////

}
}