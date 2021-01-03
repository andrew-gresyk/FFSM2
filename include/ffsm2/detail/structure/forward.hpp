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
struct WrapInfoT<	 TH> {
	using Type = SI_<TH>;
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
	using StateList			= TL_<Head>;

	static constexpr Short WIDTH			  = 1;

	static constexpr Long  STATE_COUNT	 	  = StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH			  = sizeof...(TSubStates);

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		, Long NSubstitutionLimit
		FFSM2_IF_DYNAMIC_PLANS(, Long NTaskCapacity)
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)
		, typename TPayload>
struct ArgsT final {
	using Context			= TContext;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TConfig::LoggerInterface;
#endif

	using StateList			= TStateList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

#ifdef FFSM2_ENABLE_STATIC_PLANS
	using LinksTypeTable	= TLinksTypeTable;
#endif

	using Payload			= TPayload;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct S_;

template <typename, typename, typename...>
struct C_;

template <StateID, typename, Short, typename...>
struct CS_;

template <typename, typename FFSM2_IF_STATIC_PLANS(, typename)>
class RW_;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct MaterialT;

template <StateID N, typename TA, typename TH>
struct MaterialT   <N, TA, TH> {
	using Type = S_<N, TA, TH>;
};

template <StateID N, typename TA, 			 typename... TS>
struct MaterialT   <N, TA, CI_<void,         TS...>> {
	using Type = C_<   TA, StaticEmptyT<TA>, TS...>;
};

template <StateID N, typename TA, typename TH, typename... TS>
struct MaterialT   <N, TA, CI_<TH,			  TS...>> {
	using Type = C_<   TA, TH,				  TS...>;
};

template <StateID N, typename... TS>
using Material = typename MaterialT<N, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig
		, typename TApex
		FFSM2_IF_STATIC_PLANS(, typename TLinksTypeTable)>
struct RF_ {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT = TConfig::SUBSTITUTION_LIMIT;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	static constexpr Long  TASK_CAPACITY	  = TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload		= typename TConfig::Payload;

#ifdef FFSM2_ENABLE_DYNAMIC_PLANS
	using Task			= typename TConfig::Task;
#endif

	using StateList		= typename Apex::StateList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , SUBSTITUTION_LIMIT
							  FFSM2_IF_DYNAMIC_PLANS(, TASK_CAPACITY)
							  FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<TConfig, Apex FFSM2_IF_STATIC_PLANS(, TLinksTypeTable)>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

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
	static constexpr bool	 contains() noexcept	{ return contains<StateList, T>();	}

	template <typename T>
	static constexpr StateID  stateId() noexcept	{ return index	 <StateList, T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, Short, typename>
struct CSubMaterialT;

template <StateID N, typename TA, Short NI, typename... TS>
struct CSubMaterialT<N, TA, NI, TL_<TS...>> {
	using Type = CS_<N, TA, NI,		TS...>;
};

template <StateID N, typename TA, Short NI, typename TL>
using CSubMaterial = typename CSubMaterialT<N, TA, NI, TL>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
