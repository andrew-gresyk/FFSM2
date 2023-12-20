#if FFSM2_PLANS_AVAILABLE()

namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class CPlanT {
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator final {
		FFSM2_CONSTEXPR(14)	Iterator(const CPlanT& plan)				noexcept;

		FFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		FFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		FFSM2_CONSTEXPR(11)	const Task& operator  *()			  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		FFSM2_CONSTEXPR(11)	const Task* operator ->()			  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		FFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	FFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData)				noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds}
	{}

	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID stateId()								noexcept	{ return index<StateList , TState>();		}

public:
	FFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	FFSM2_CONSTEXPR(14)	Iterator begin()								noexcept	{ return Iterator{*this};					}

	/// @brief Iteration terminator
	/// @return Dummy Iterator
	FFSM2_CONSTEXPR(14)	Iterator end  ()								noexcept	{ return Iterator{*this};					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief First task
	/// @return First task
	FFSM2_CONSTEXPR(14) const Task& first()						  const noexcept;

	/// @brief Last task
	/// @return Last task
	FFSM2_CONSTEXPR(14) const Task&  last()						  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_0.inl"
