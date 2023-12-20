namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <StateID, typename, typename>
	friend struct S_;

	template <typename, typename, typename...>
	friend struct C_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;

	using Core				= CoreT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		FFSM2_CONSTEXPR(14)	 Origin(ControlT& control_,
									const StateID stateId_)					noexcept;

		FFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(11)	ControlT(Core& core)								noexcept
		: _core{core}
	{}

public:

#if FFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	FFSM2_CONSTEXPR(11) StateID stateId()							  const noexcept	{ return _originId;							}

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()									noexcept	{ return index<StateList , TState>();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::context()`
	FFSM2_CONSTEXPR(14)		  Context& _()									noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::context()`
	FFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::_()`
	FFSM2_CONSTEXPR(14)		  Context& context()							noexcept	{ return _core.context;						}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::_()`
	FFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Inspect current transition request
	/// @return Transition requests
	FFSM2_CONSTEXPR(11)	const Transition& request()					  const noexcept	{ return _core.request;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param `stateId` State identifier
	/// @return State active status
	FFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);	}

	/// @brief Check if a state is active
	/// @tparam `TState` State type
	/// @return State active status
	template <typename TState>
	FFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan
	/// @return Plan
	/// @see `FFSM2_ENABLE_PLANS`
	FFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.planData};				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last `update()`, `react()` or `replayTransition()`
	/// @return Array of last transition requests
	/// @see `FFSM2_ENABLE_TRANSITION_HISTORY`
	FFSM2_CONSTEXPR(11)	const Transition& previousTransitions()		  const noexcept	{ return _core.previousTransition;			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	Core& _core;
	StateID _originId = INVALID_STATE_ID;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_1.inl"
