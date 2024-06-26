﻿namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct CI_;

template <typename>
struct CSI_;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT	<TH> final {
	using Type = SI_<TH>;
};

template <typename TH, typename... TS>
struct WrapInfoT<CI_<TH, TS...>> final {
	using Type = CI_<TH, TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------
// COMMON

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;

	static constexpr Short WIDTH		  = 1;

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename THead, typename... TSubStates>
struct CI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TL_<TSubStates...>>;
	using StateList			= typename SubStates::StateList;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  WIDTH_BITS	  = static_cast<Long>(bitWidth(WIDTH));
	static constexpr Long  ACTIVE_BITS	  = WIDTH_BITS;
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, typename... TR>
struct CSI_<TL_<TI, TR...>> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= CSI_<TL_<TR...>>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;
};

template <typename TI>
struct CSI_<TL_<TI>> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;

	static constexpr Long  STATE_COUNT	  =  StateList::SIZE;
};

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

////////////////////////////////////////////////////////////////////////////////

template <
	typename TConfig
  , typename TStateList
  FFSM2_IF_SERIALIZATION(, Long NSerialBits)
  FFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct ArgsT final {
	using Config		= TConfig;
	using Context		= typename Config::Context;
	using PureContext	= Undecorate<Context>;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename Config::LoggerInterface;
#endif

	using StateList		= TStateList;

	static constexpr Long  STATE_COUNT			= StateList ::SIZE;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short SERIAL_BITS			= NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT	= Config::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY		= NTaskCapacity;
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

template <StateID, typename, Short, typename>
struct CS_;

template <typename, typename>
class InstanceT;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct MaterialT_;

template <StateID NN, typename TA, typename TH>
struct MaterialT_  <NN, TA, TH> final {
	using Type = S_<NN, TA, TH>;
};

template <StateID NN, typename TA, 				typename... TS>
struct MaterialT_  <NN, TA, CI_<void	  , TS...>> {
	using Type = C_<	TA,		EmptyT<TA>, TS...>;
};

template <StateID NN, typename TA, typename TH, typename... TS>
struct MaterialT_  <NN, TA, CI_<TH, TS...>> {
	using Type = C_<	TA,		TH, TS...>;
};

template <StateID NN, typename... TS>
using MaterialT = typename MaterialT_<NN, TS...>::Type;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TApex
>
struct RF_ final {
	using Config			= TConfig;
	using Context			= typename Config::Context;
	using Apex				= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using StateList			= typename Apex::StateList;

	static constexpr Long  STATE_COUNT			= Apex::STATE_COUNT;

	static constexpr Short SUBSTITUTION_LIMIT	= Config::SUBSTITUTION_LIMIT;

#if FFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY		= Config::TASK_CAPACITY != INVALID_LONG ?
													  Config::TASK_CAPACITY : Apex::STATE_COUNT;
#endif

	using Payload			= typename Config::Payload;
	using Transition		= TransitionT<Payload>;

#if FFSM2_PLANS_AVAILABLE()
	using Task				= typename Config::Task;
#endif

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
	static constexpr Long  SERIAL_BITS			= 1 + ACTIVE_BITS;
#endif

	using Args				= ArgsT<
								  TConfig
								, StateList
								FFSM2_IF_SERIALIZATION(, SERIAL_BITS)
								FFSM2_IF_PLANS(, TASK_CAPACITY)
								, Payload
							  >;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance			= InstanceT<Config, Apex>;

	using ConstControl		= ConstControlT<Args>;
	using Control			= ControlT	   <Args>;
	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using State				= EmptyT	   <Args>;

	template <typename... TInjections>
	using StateT			= A_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename Config::LoggerInterface;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	static constexpr bool	  contains()		noexcept	{ return					contains<StateList , TState>() ;	}

	template <typename TState>
	static constexpr StateID  stateId()			noexcept	{ return					   index<StateList , TState>() ;	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, Short, typename>
struct LHalfCST;

template <StateID NN, typename TA, Short NI, typename... TS>
struct LHalfCST<NN, TA, NI, TL_<TS...>> final {
	using Type = CS_<NN,
					 TA,
					 NI,
					 LHalfTypes<TS...>>;
};

template <StateID NN, typename TA, Short NI, typename TL>
using LHalfCS = typename LHalfCST<NN, TA, NI, TL>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID, typename, Short, typename>
struct RHalfCST;

template <StateID NN, typename TA, Short NI, typename... TS>
struct RHalfCST<NN, TA, NI, TL_<TS...>> final {
	using Type = CS_<NN + sizeof...(TS) / 2,
					 TA,
					 NI + sizeof...(TS) / 2,
					 RHalfTypes<TS...>>;
};

template <StateID NN, typename TA, Short NI, typename TL>
using RHalfCS = typename RHalfCST<NN, TA, NI, TL>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
