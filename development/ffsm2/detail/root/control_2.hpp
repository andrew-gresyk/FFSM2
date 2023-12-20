namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

	template <typename, typename>
	friend class RV_;

protected:
	using Control			= ControlT<TArgs>;

	using typename Control::StateList;

	using typename Control::Core;

	using Transition		= typename Core::Transition;

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		FFSM2_CONSTEXPR(14)	Region(PlanControlT& control)					noexcept;

		FFSM2_CONSTEXPR(20)	~Region()										noexcept;

		PlanControlT& control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	PlanControlT(Core& core,
									 const Transition& currentTransition)	noexcept
		: Control{core}
		, _currentTransition{currentTransition}
	{}

	FFSM2_CONSTEXPR(14)	void   setRegion()									noexcept;
	FFSM2_CONSTEXPR(14)	void resetRegion()									noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()
	using typename Control::CPlan;

	using Plan			= PayloadPlanT<TArgs>;

	/// @brief Access plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(14)	 Plan plan()										noexcept	{ return  Plan{_core.planData};		}

// COMMON
// COMMON

	/// @brief Access read-only plan
	/// @return Read-only plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.planData};		}

// COMMON
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get current transition request
	/// @return Current transition request
	FFSM2_CONSTEXPR(11)	const Transition& currentTransition()		  const noexcept	{ return _currentTransition;	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_core;

	const Transition& _currentTransition;

	TaskStatus _taskStatus;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_2.inl"
