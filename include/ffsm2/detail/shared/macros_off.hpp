//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

#if _MSC_VER == 1900
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

#undef FFSM2_CONSTEXPR
#undef FFSM2_CONSTEXPR_11
#undef FFSM2_CONSTEXPR_14
#undef FFSM2_CONSTEXPR_17

//#undef FFSM2_ARCHITECTURE
//#undef FFSM2_ARCHITECTURE_64
//#undef FFSM2_ARCHITECTURE_32
#undef FFSM2_64BIT_OR_32BIT

//#undef FFSM2_BREAK
#undef FFSM2_BREAK_AVAILABLE

#undef FFSM2_IF_DEBUG
#undef FFSM2_UNLESS_DEBUG
#undef FFSM2_DEBUG_OR

#undef FFSM2_ASSERT_AVAILABLE
#undef FFSM2_IF_ASSERT
//#undef FFSM2_CHECKED
#undef FFSM2_ASSERT
#undef FFSM2_ASSERT_OR

#undef FFSM2_IF_TYPEINDEX
#undef FFSM2_TYPEINDEX_AVAILABLE
#undef FFSM2_IF_TYPEINDEX

//#undef FFSM2_DEBUG_STATE_TYPE_AVAILABLE

//#undef FFSM2_PLANS_AVAILABLE
#undef FFSM2_IF_PLANS

//#undef FFSM2_SERIALIZATION_AVAILABLE
#undef FFSM2_IF_SERIALIZATION

//#undef FFSM2_STRUCTURE_REPORT_AVAILABLE
//#undef FFSM2_IF_STRUCTURE_REPORT

//#undef FFSM2_TRANSITION_HISTORY_AVAILABLE
#undef FFSM2_IF_TRANSITION_HISTORY

#undef FFSM2_VERBOSE_DEBUG_LOG_AVAILABLE
#undef FFSM2_LOG_INTERFACE_AVAILABLE
#undef FFSM2_IF_LOG_INTERFACE

#undef FFSM2_LOG_TRANSITION

#if FFSM2_PLANS_AVAILABLE()
	#undef FFSM2_LOG_TASK_STATUS
	#undef FFSM2_LOG_PLAN_STATUS
#endif

#undef FFSM2_LOG_CANCELLED_PENDING

#undef FFSM2_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////
