// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

namespace test_tools {

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
void
LoggerT<TConfig>::recordMethod(const Context& /*context*/,
							   const StateID origin,
							   const Method method)
{
	REQUIRE(ffsm2::methodName(method));

	switch (method) {

		case Method::ENTRY_GUARD:
			history.emplace_back(origin, Event::Type::ENTRY_GUARD);
			break;

		case Method::ENTER:
			history.emplace_back(origin, Event::Type::ENTER);
			break;

	//#ifdef FFSM2_ENABLE_REENTER

		case Method::REENTER:
			history.emplace_back(origin, Event::Type::REENTER);
			break;

	//#endif

		case Method::PRE_UPDATE:
			history.emplace_back(origin, Event::Type::PRE_UPDATE);
			break;

		case Method::UPDATE:
			history.emplace_back(origin, Event::Type::UPDATE);
			break;

		case Method::POST_UPDATE:
			history.emplace_back(origin, Event::Type::POST_UPDATE);
			break;

		case Method::PRE_REACT:
			history.emplace_back(origin, Event::Type::PRE_REACT);
			break;

		case Method::REACT:
			history.emplace_back(origin, Event::Type::REACT);
			break;

		case Method::POST_REACT:
			history.emplace_back(origin, Event::Type::POST_REACT);
			break;

		case Method::EXIT_GUARD:
			history.emplace_back(origin, Event::Type::EXIT_GUARD);
			break;

		case Method::EXIT:
			history.emplace_back(origin, Event::Type::EXIT);
			break;

	//#ifdef FFSM2_ENABLE_PLANS

		case Method::PLAN_SUCCEEDED:
			history.emplace_back(origin, Event::Type::PLAN_SUCCEEDED);
			break;

		case Method::PLAN_FAILED:
			history.emplace_back(origin, Event::Type::PLAN_FAILED);
			break;

	//#endif

		default:
			assert(false);
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordTransition(const Context& UNUSED(context),
								   const StateID origin,
								   const StateID target)
{
	history.emplace_back(origin, Event::Type::CHANGE, target);
}

//------------------------------------------------------------------------------

#ifdef FFSM2_ENABLE_PLANS

template <typename TConfig>
void
LoggerT<TConfig>::recordTaskStatus(const Context& UNUSED(context),
								   const StateID origin,
								   const StatusEvent event)
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(origin, Event::Type::TASK_SUCCESS);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(origin, Event::Type::TASK_FAILURE);
			break;

		default:
			assert(false);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TConfig>
void
LoggerT<TConfig>::recordPlanStatus(const Context& UNUSED(context),
								   const StatusEvent event)
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(Event::Type::PLAN_SUCCESS, ffsm2::INVALID_STATE_ID);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(Event::Type::PLAN_FAILURE, ffsm2::INVALID_STATE_ID);
			break;

		default:
			assert(false);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordCancelledPending(const Context& UNUSED(context),
										 const StateID origin)
{
	history.emplace_back(origin, Event::Type::CANCEL_PENDING);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::assertSequence(const Events& reference) {
	const auto count = ffsm2::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history  .size());
		REQUIRE(i < reference.size());

		if (i < history	 .size() &&
			i < reference.size())
		{
			const Event h = history  [i];
			const Event r = reference[i];

			REQUIRE(h.type	 == r.type	);
			REQUIRE(h.origin == r.origin);
			REQUIRE(h.target == r.target);
		}
	}

	history.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
