namespace ffsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename TArgs
  , typename THead
  , typename... TSubStates
>
struct FFSM2_EMPTY_BASES C_
	: S_<INVALID_STATE_ID, TArgs, THead>
	, CS_<
		  0
		, TArgs
		, 0
		, TL_<TSubStates...>
	  >
{
	using Args			= TArgs;

	using HeadState		= S_<INVALID_STATE_ID, Args, THead>;
	using SubStates		= CS_<0, Args, 0, TL_<TSubStates...>>;

	using StateList		= typename Args::StateList;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT <Args>;
	using ScopedRegion	= typename PlanControl::Region;

#if FFSM2_PLANS_AVAILABLE()
	using Plan			= typename PlanControl::Plan;
#endif

	using FullControl	= FullControlT <Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

#if FFSM2_PLANS_AVAILABLE()
	using PlanData		= typename Control::PlanData;
#endif

	using Head			= THead;
	using Info			= CI_<Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;

#if FFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_SERIALIZATION_AVAILABLE()
	FFSM2_CONSTEXPR(11)	static Prong		compoRequested		  (const Registry& registry)	noexcept	{ return registry.requested;	}
	FFSM2_CONSTEXPR(11)	static Prong&		compoRequested		  (		 Registry& registry)	noexcept	{ return registry.requested;	}
#endif

	FFSM2_CONSTEXPR(11)	static Prong&		compoRequested		  (		Control& control)		noexcept	{ return control._core.registry.requested;	}

	FFSM2_CONSTEXPR(11)	static Prong&		compoActive			  (		Control& control)		noexcept	{ return control._core.registry.active;		}
	FFSM2_CONSTEXPR(11)	static Prong		compoActive			  (ConstControl& control)		noexcept	{ return control._core.registry.active;		}

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(11)	static TaskStatus&	headStatus			  (		Control& control)		noexcept	{ return control._core.planData.headStatus;	}
	FFSM2_CONSTEXPR(11)	static TaskStatus&	 subStatus			  (		Control& control)		noexcept	{ return control._core.planData.subStatus;	}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	bool		deepForwardEntryGuard		  (GuardControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	bool		deepEntryGuard				  (GuardControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepEnter					  ( PlanControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepReenter					  ( PlanControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepPreUpdate				  ( FullControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepUpdate					  ( FullControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	void		deepPostUpdate				  ( FullControl& control					 )			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepPreReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepReact					  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepPostReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	FFSM2_CONSTEXPR(14)	void		deepQuery					  (ConstControl& control,		TEvent& event)	  const noexcept;

#if FFSM2_PLANS_AVAILABLE()
	FFSM2_CONSTEXPR(14)	void		deepUpdatePlans				  ( FullControl& control					 )			noexcept;
#endif

	FFSM2_CONSTEXPR(14)	bool		deepForwardExitGuard		  (GuardControl& control					 )			noexcept;
	FFSM2_CONSTEXPR(14)	bool		deepExitGuard				  (GuardControl& control					 )			noexcept;

	FFSM2_CONSTEXPR(14)	void		deepExit					  ( PlanControl& control					 )			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	FFSM2_CONSTEXPR(14)	void		deepChangeToRequested		  ( PlanControl& control					 )			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if FFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	FFSM2_CONSTEXPR(14)	void		deepSaveActive				  (const Registry& registry, WriteStream& stream) const noexcept;
	FFSM2_CONSTEXPR(14)	void		deepLoadRequested			  (		 Registry& registry,  ReadStream& stream) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
