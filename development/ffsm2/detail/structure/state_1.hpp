namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	StateID NStateId
  , typename TArgs
  , typename THead
>
struct S_
	: THead
{
	static constexpr StateID STATE_ID = NStateId;

	using Context		= typename TArgs::Context;

#if FFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TArgs::Logger;
#endif

	using ConstControl	= ConstControlT<TArgs>;
	using ScopedCOrigin	= typename ConstControl::Origin;

	using Control		= ControlT	   <TArgs>;

	using PlanControl	= PlanControlT <TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Empty			= EmptyT<TArgs>;
	using Head			= THead;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool		deepEntryGuard		 (GuardControl&	control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepEnter			 ( PlanControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepReenter			 ( PlanControl& control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	TaskStatus	deepPreUpdate		 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	deepUpdate			 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPostUpdate		 ( FullControl& control						 )			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPreReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepReact			 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	TaskStatus	deepPostReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepQuery			 (ConstControl& control,	   TEvent&  event)	  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	TaskStatus	deepUpdatePlans		 ( FullControl& control)								noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		deepExitGuard		 (GuardControl&	control						 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepExit			 ( PlanControl& control						 )			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void		wrapPlanSucceeded	 ( FullControl& control						 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		wrapPlanFailed		 ( FullControl& control						 )			noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void		deepChangeToRequested(	   Control&								 )			noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_DEBUG_STATE_TYPE_AVAILABLE() || FFSM2_STRUCTURE_REPORT_AVAILABLE() || FFSM2_LOG_INTERFACE_AVAILABLE()

	FFSM2_IF_TYPEINDEX(const std::type_index TYPE = typeid(Head));

#endif

#if FFSM2_LOG_INTERFACE_AVAILABLE()

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename THost, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...),
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	template <typename TReturn, typename THost, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...) const,
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	template <typename TReturn, typename... TParams>
	FFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...) const,
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state_1.inl"
