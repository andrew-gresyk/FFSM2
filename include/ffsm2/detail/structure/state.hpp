namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NStateId,
		  typename TArgs,
		  typename THead>
struct S_ final {
	static constexpr auto STATE_ID	 = NStateId;

	using Context		= typename TArgs::Context;

#ifdef FFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TArgs::Logger;
#endif

	using Control		= ControlT<TArgs>;

	using PlanControl	= PlanControlT<TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;
	using HeadBox		= Boxify<Head, TArgs>;

	//----------------------------------------------------------------------

	constexpr bool	 deepEntryGuard		  (GuardControl& control) noexcept;

	constexpr void	 deepConstruct		  (PlanControl&  control) noexcept;

	constexpr void	 deepEnter			  (PlanControl&  control) noexcept;
	constexpr void	 deepReenter		  (PlanControl&  control) noexcept;

	constexpr Status deepUpdate			  (FullControl&  control) noexcept;

	template <typename TEvent>
	constexpr Status deepReact			  (FullControl&	 control,
										   const TEvent& event)	  noexcept;

	constexpr bool	 deepExitGuard		  (GuardControl& control) noexcept;

	constexpr void	 deepExit			  (PlanControl&	 control) noexcept;

	constexpr void	 deepDestruct		  (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_PLANS
	constexpr void	 wrapPlanSucceeded	  (FullControl&	 control) noexcept;
	constexpr void	 wrapPlanFailed		  (FullControl&	 control) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	constexpr void	 deepChangeToRequested(Control&)			  noexcept {}

	//----------------------------------------------------------------------

#if defined _DEBUG || defined FFSM2_ENABLE_STRUCTURE_REPORT || defined FFSM2_ENABLE_LOG_INTERFACE

	static constexpr Long NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

	//----------------------------------------------------------------------

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Empty			= EmptyT<TArgs>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename THost, typename... TParams>
	constexpr void log(TReturn(THost::*)(TParams...),
					   Logger& logger,
					   Context& context,
					   const Method method) const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	constexpr void log(TReturn(Empty::*)(TParams...),
					   Logger&,
					   Context&,
					   const Method) const noexcept
	{}

#endif

	//----------------------------------------------------------------------

	// TODO: account for boxing
	//
	// if you see..
	// VS	 - error C2079: 'ffsm2::detail::S_<...>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'ffsm2::detail::S_<...>::Head' (aka 'Blah')
	//
	// .. add definition for the state 'Blah'
	HeadBox _headBox;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state.inl"
