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
struct WrapInfoT<	  TH> {
	using Type =  SI_<TH>;
};

template <typename TH, typename... TS>
struct WrapInfoT< CI_<TH, TS...>> {
	using Type =  CI_<TH, TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= ITL_<Head>;

	static constexpr Short WIDTH		  = 1;

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		, typename TPayload>
struct ArgsT final {
	using Context		= TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

	using Payload	= TPayload;
};

//------------------------------------------------------------------------------

template <StateID NStateID>
struct I_ {
	static constexpr StateID STATE_ID	 = NStateID;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <typename, typename, Short, typename...>
struct CS_;

template <typename, typename>
class RW_;

//------------------------------------------------------------------------------

template <typename, typename...>
struct MaterialT;

template <typename TN, typename TA, typename TH>
struct MaterialT   <TN, TA, TH> {
	using Type = S_<TN, TA, TH>;
};

template <typename TN, typename TA, 			 typename... TS>
struct MaterialT   <TN, TA, CI_<void,         TS...>> {
	using Type = C_<	TA, StaticEmptyT<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT   <TN, TA, CI_<TH,			  TS...>> {
	using Type = C_<	TA, TH,				  TS...>;
};

template <typename TN, typename... TS>
using Material = typename MaterialT<TN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT	= TConfig::SUBSTITUTION_LIMIT;

	using Payload	= typename TConfig::Payload;

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , SUBSTITUTION_LIMIT
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<TConfig, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	//using Injection		= InjectionT<Args>;

	//----------------------------------------------------------------------

	using DynamicState	= DynamicEmptyT<Args>;

	template <typename... TInjections>
	using DynamicStateT	= DB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using StaticState	= StaticEmptyT<Args>;

	template <typename... TInjections>
	using StaticStateT	= SB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using State			= StaticState;

	template <typename... TInjections>
	using StateT		= StaticStateT<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TConfig::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG;
	}

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Short NI, typename T>
struct CSubMaterialT;

template <typename TN, typename TA, Short NI, typename... TS>
struct CSubMaterialT<TN, TA, NI, ITL_<TS...>> {
	using Type = CS_<TN, TA, NI,	  TS...>;
};

template <typename TN, typename TA, Short NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct InfoT;

template <typename TN, typename TA, typename TH>
struct InfoT<S_<TN, TA, TH>> {
	using Type = SI_<	TH>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct InfoT<C_<TN, TA, TH, TS...>> {
	using Type = CI_<	TH, TS...>;
};

template <typename TN, typename TA, Short NI, typename... TS>
struct InfoT<CS_<TN, TA, NI, TS...>> {
	using Type = CSI_<		 TS...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}
