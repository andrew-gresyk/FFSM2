namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

// Automatic / manual [de]activation

template <
	typename
  , typename
>
class RV_;

//------------------------------------------------------------------------------
// Automatic enter() / exit()

template <
	FeatureTag NFeatureTag
  , typename TContext
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES RV_<
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			R_ <
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		R_ <
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if FFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	FFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   FFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	FFSM2_CONSTEXPR(14)	RV_(const RV_&  other)													noexcept;
	FFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)													noexcept;

	FFSM2_CONSTEXPR(20)	~RV_()																	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;
#endif

private:
	using Base::initialEnter;
	using Base::finalExit;

protected:
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_core;
	using Base::_apex;
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Manual enter() / exit()

template <
	FeatureTag NFeatureTag
  , typename TContext
  , Short NSubstitutionLimit
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class FFSM2_EMPTY_BASES RV_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			R_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		R_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , NSubstitutionLimit
							  FFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

protected:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::PlanControl;

	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

public:
	using typename Base::Transition;

private:
#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
#endif

public:
	using Base::Base;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if FSM is active
	/// @return FSM active status
	FFSM2_CONSTEXPR(11)	bool isActive()												  const noexcept	{ return _core.registry.isActive();	}

	using Base::isActive;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Manually start the FSM
	///	  Can be used with UE4 to start / stop the FSM in `BeginPlay()` / `EndPlay()`
	FFSM2_CONSTEXPR(14)	void enter()														noexcept	{ initialEnter();	}

	/// @brief Manually stop the FSM
	///   Can be used with UE4 to start / stop the FSM in `BeginPlay()` / `EndPlay()`
	FFSM2_CONSTEXPR(14)	void exit()															noexcept	{ finalExit();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `FFSM2_ENABLE_SERIALIZATION`
	FFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///   Can be used with UE4 USTRUCT() NetSerialize() to load replicated FSM from FArchive
	/// @param `destination` Transition destination
	/// @see `FFSM2_ENABLE_TRANSITION_HISTORY`
	FFSM2_CONSTEXPR(14)	void replayEnter(const StateID destination)							noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
#if FFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;

	FFSM2_CONSTEXPR(14)	void loadEnter(ReadStream& stream)										noexcept;
#endif

protected:
	using Base::initialEnter;
	using Base::finalExit;

	using Base::_core;

#if FFSM2_SERIALIZATION_AVAILABLE() || FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_apex;
#endif

#if FFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::applyRequest;
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_1.inl"
