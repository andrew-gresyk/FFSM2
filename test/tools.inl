// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
void
LoggerT<TConfig>::recordMethod(Context& /*context*/,
							   const StateID origin,
							   const Method method) noexcept
{
	REQUIRE(ffsm2::methodName(method));

	switch (method) {

		case Method::ENTRY_GUARD:
			history.emplace_back(origin, Event::Type::ENTRY_GUARD);
			break;

		case Method::ENTER:
			history.emplace_back(origin, Event::Type::ENTER);
			break;

		case Method::REENTER:
			history.emplace_back(origin, Event::Type::REENTER);
			break;

		case Method::UPDATE:
			history.emplace_back(origin, Event::Type::UPDATE);
			break;

		case Method::REVERSE_UPDATE:
			history.emplace_back(origin, Event::Type::REVERSE_UPDATE);
			break;

		case Method::REACT:
			history.emplace_back(origin, Event::Type::REACT);
			break;

		case Method::REVERSE_REACT:
			history.emplace_back(origin, Event::Type::REVERSE_REACT);
			break;

		case Method::EXIT_GUARD:
			history.emplace_back(origin, Event::Type::EXIT_GUARD);
			break;

		case Method::EXIT:
			history.emplace_back(origin, Event::Type::EXIT);
			break;

	#if FFSM2_PLANS_AVAILABLE()

		case Method::PLAN_SUCCEEDED:
			history.emplace_back(origin, Event::Type::PLAN_SUCCEEDED);
			break;

		case Method::PLAN_FAILED:
			history.emplace_back(origin, Event::Type::PLAN_FAILED);
			break;

	#endif

		default:
			FFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordTransition(Context& /*context*/,
								   const StateID origin,
								   const StateID target) noexcept
{
	history.emplace_back(origin, Event::Type::CHANGE, target);
}

//------------------------------------------------------------------------------

#if FFSM2_PLANS_AVAILABLE()

template <typename TConfig>
void
LoggerT<TConfig>::recordTaskStatus(Context& /*context*/,
								   const StateID origin,
								   const StatusEvent event) noexcept
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(origin, Event::Type::TASK_SUCCESS);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(origin, Event::Type::TASK_FAILURE);
			break;

		default:
			FFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordPlanStatus(Context& /*context*/,
								   const StatusEvent event) noexcept
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(Event::Type::PLAN_SUCCESS, ffsm2::INVALID_STATE_ID);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(Event::Type::PLAN_FAILURE, ffsm2::INVALID_STATE_ID);
			break;

		default:
			FFSM2_BREAK();
	}
}

#endif


//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordCancelledPending(Context& /*context*/,
										 const StateID origin) noexcept
{
	history.emplace_back(origin, Event::Type::CANCEL_PENDING);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::assertSequence(const Events& reference) noexcept {
	const auto count = std::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history.size()); //-V521
		REQUIRE(i < reference.size()); //-V521

		if (i < history.size() &&
			i < reference.size())
		{
			REQUIRE(history[i].type	  == reference[i].type); //-V521
			REQUIRE(history[i].origin == reference[i].origin); //-V521
			REQUIRE(history[i].target == reference[i].target); //-V521
		}
	}

	history.clear();
}

////////////////////////////////////////////////////////////////////////////////
