namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class B_ {
	template <typename...>
	friend struct A_;

protected:
	using Context		= typename TArgs::Context;

	using StateList		= typename TArgs::StateList;

	using ConstControl	= ConstControlT<TArgs>;
	using Control		= ControlT	   <TArgs>;
	using PlanControl	= PlanControlT <TArgs>;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= PayloadPlanT <TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)			noexcept	{}
	FFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void	query		  (		 TEvent&		,
											   ConstControl&		)	  const noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)			noexcept	{}

	FFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)			noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	static
	FFSM2_CONSTEXPR(11) StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}
};

////////////////////////////////////////////////////////////////////////////////

}
}
