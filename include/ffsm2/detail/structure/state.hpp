namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ final {
	static constexpr auto STATE_ID	 = TIndices::STATE_ID;

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

	FFSM2_INLINE bool	 deepEntryGuard		  (GuardControl& control);

	FFSM2_INLINE void	 deepConstruct		  (PlanControl&  control);

	FFSM2_INLINE void	 deepEnter			  (PlanControl&  control);
	FFSM2_INLINE void	 deepReenter		  (PlanControl&  control);

	FFSM2_INLINE Status	 deepUpdate			  (FullControl&  control);

	template <typename TEvent>
	FFSM2_INLINE Status	 deepReact			  (FullControl&	 control, const TEvent& event);

	FFSM2_INLINE bool	 deepExitGuard		  (GuardControl& control);

	FFSM2_INLINE void	 deepExit			  (PlanControl&	 control);

	FFSM2_INLINE void	 deepDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_INLINE void	deepChangeToRequested(Control&)																		{}

	//----------------------------------------------------------------------

#if defined _DEBUG || defined FFSM2_ENABLE_STRUCTURE_REPORT || defined FFSM2_ENABLE_LOG_INTERFACE

	static constexpr Long NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef FFSM2_ENABLE_LOG_INTERFACE

	template <typename>
	struct Traits_;

	template <typename TR_, typename TH_, typename... TAs_>
	struct Traits_<TR_(TH_::*)(TAs_...)> {
		using Host = TH_;
	};

	template <typename TM_>
	using Host_			= typename Traits_<TM_>::Host;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Empty			= EmptyT<TArgs>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TMethodType>
	typename std::enable_if<
				 std::is_same<
					 Host_<TMethodType>,
					 Empty
				 >::value
			 >::type
	log(Logger&,
		Context&,
		const Method) const
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TMethodType>
	typename std::enable_if<
				 !std::is_same<
					 Host_<TMethodType>,
					 Empty
				 >::value
			 >::type
	log(Logger& logger,
		Context& context,
		const Method method) const
	{
		logger.recordMethod(context, STATE_ID, method);
	}

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
