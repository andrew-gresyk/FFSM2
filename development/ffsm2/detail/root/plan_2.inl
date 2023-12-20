#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TSL_ FFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
FFSM2_CONSTEXPR(14)
bool
PayloadPlanT<ArgsT<TG_, TSL_ FFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::append(const StateID origin,
																				  const StateID destination,
																				  const Payload& payload) noexcept
{
	_planData.planExists = true;

	return linkTask(_planData.tasks.emplace(origin, destination, payload));
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
