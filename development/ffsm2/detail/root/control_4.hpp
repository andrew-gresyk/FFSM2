namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Core;
	using typename FullControl::Context;

	using typename FullControl::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	GuardControlT(Core& core,
									  const Transition& currentTransition,
									  const Transition& pendingTransition) noexcept
		: FullControl{core, currentTransition}
		, _pendingTransition{pendingTransition}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get pending transition request
	/// @return Pending transition request
	FFSM2_CONSTEXPR(11)	const Transition& pendingTransition()	  const noexcept	{ return _pendingTransition;	}

	/// @brief Cancel pending transition request
	///   (can be used to substitute a transition into the current state with a different one)
	FFSM2_CONSTEXPR(14)	void cancelPendingTransition()					noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using FullControl::_core;
	using FullControl::_originId;

	const Transition& _pendingTransition;

	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_4.inl"
