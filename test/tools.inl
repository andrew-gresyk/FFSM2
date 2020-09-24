////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
void
LoggerT<TConfig>::recordMethod(Context& /*context*/,
							   const StateID origin,
							   const Method method)
{
	REQUIRE(ffsm2::methodName(method));

	switch (method) {

		case Method::ENTRY_GUARD:
			history.emplace_back(origin, Event::ENTRY_GUARD);
			break;

		case Method::CONSTRUCT:
			history.emplace_back(origin, Event::CONSTRUCT);
			break;

		case Method::ENTER:
			history.emplace_back(origin, Event::ENTER);
			break;

		case Method::REENTER:
			history.emplace_back(origin, Event::REENTER);
			break;

		case Method::UPDATE:
			history.emplace_back(origin, Event::UPDATE);
			break;

		case Method::REACT:
			history.emplace_back(origin, Event::REACT);
			break;

		case Method::EXIT_GUARD:
			history.emplace_back(origin, Event::EXIT_GUARD);
			break;

		case Method::EXIT:
			history.emplace_back(origin, Event::EXIT);
			break;

		case Method::DESTRUCT:
			history.emplace_back(origin, Event::DESTRUCT);
			break;

		default:
			FFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordTransition(Context& /*context*/,
								   const StateID origin,
								   const StateID target)
{
	history.emplace_back(origin, Event::CHANGE, target);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordCancelledPending(Context& /*context*/,
										 const StateID origin)
{
	history.emplace_back(origin, Event::CANCEL_PENDING);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::assertSequence(const Events& reference) {
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
