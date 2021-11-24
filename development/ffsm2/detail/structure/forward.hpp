namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct CI_;

template <typename...>
struct CSI_;

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct CSI_<TInitial>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> final {
	using Type = SI_<TH>;
};

template <typename TH, typename... TS>
struct WrapInfoT< CI_<TH, TS...>> final {
	using Type =  CI_<TH, TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------
// COMMON

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;

	static constexpr Short WIDTH		= 1;

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> final {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> final {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH		= sizeof...(TSubStates);

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  WIDTH_BITS	= (Long) bitWidth(WIDTH);
	static constexpr Long  ACTIVE_BITS	= WIDTH_BITS;
#endif

	static constexpr Long  STATE_COUNT	= StateList::SIZE;
};

// COMMON
////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		FFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		FFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct ArgsT final {
	using Context		= TContext;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short SERIAL_BITS		  = NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload		= TPayload;

#if FFSM2_SERIALIZATION_AVAILABLE()
	using SerialBuffer	= StreamBufferT  <SERIAL_BITS>;
	using WriteStream	= BitWriteStreamT<SERIAL_BITS>;
	using ReadStream	= BitReadStreamT <SERIAL_BITS>;
#endif
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <StateID, typename, Short, typename...>
struct CS_;

template <typename, typename>
class RC_;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct MaterialT;

template <StateID N, typename TA, typename TH>
struct MaterialT   <N, TA, TH> final {
	using Type = S_<N, TA, TH>;
};

template <StateID N, typename TA, 			   typename... TS>
struct MaterialT   <N, TA, CI_<void,   TS...>> final {
	using Type = C_<   TA, EmptyT<TA>, TS...>;
};

template <StateID N, typename TA, typename TH, typename... TS>
struct MaterialT   <N, TA, CI_<TH,	   TS...>> final {
	using Type = C_<   TA,	   TH,	   TS...>;
};

template <StateID N, typename... TS>
using Material = typename MaterialT<N, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT = TConfig::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY	  = TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload		= typename TConfig::Payload;
	using Transition	= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using Task			= typename TConfig::Task;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
#endif

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  FFSM2_IF_SERIALIZATION(, ACTIVE_BITS)
							  , SUBSTITUTION_LIMIT
							  FFSM2_IF_PLANS(, TASK_CAPACITY)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RC_<TConfig, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	//----------------------------------------------------------------------

	using State			= EmptyT<Args>;

	template <typename... TInjections>
	using StateT		= B_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	template <typename T>
	static constexpr bool	 contains() noexcept	{ return contains<StateList, T>();	}

	template <typename T>
	static constexpr StateID  stateId() noexcept	{ return index	 <StateList, T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, Short, typename>
struct CSubMaterialT;

template <StateID N, typename TA, Short NI, typename... TS>
struct CSubMaterialT<N, TA, NI, TL_<TS...>> final {
	using Type = CS_<N, TA, NI,		TS...>;
};

template <StateID N, typename TA, Short NI, typename TL>
using CSubMaterial = typename CSubMaterialT<N, TA, NI, TL>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
