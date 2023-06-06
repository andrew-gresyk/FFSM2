// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define FFSM2_ENABLE_SERIALIZATION
#define FFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_serialization {

////////////////////////////////////////////////////////////////////////////////

using Config = ffsm2::Config
					::ManualActivation;

using M = ffsm2::MachineT<Config>;

//------------------------------------------------------------------------------

namespace server {

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B),
				S(C),
				S(D),
				S(E),
				S(F),
				S(G),
				S(H),
				S(I)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");
static_assert(FSM::stateId<C>() == 2, "");
static_assert(FSM::stateId<D>() == 3, "");
static_assert(FSM::stateId<E>() == 4, "");
static_assert(FSM::stateId<F>() == 5, "");
static_assert(FSM::stateId<G>() == 6, "");
static_assert(FSM::stateId<H>() == 7, "");
static_assert(FSM::stateId<I>() == 8, "");

//------------------------------------------------------------------------------

struct A : FSM::State {};
struct B : FSM::State {};
struct C : FSM::State {};
struct D : FSM::State {};
struct E : FSM::State {};
struct F : FSM::State {};
struct G : FSM::State {};
struct H : FSM::State {};
struct I : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace client {

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B),
				S(C),
				S(D),
				S(E),
				S(F),
				S(G),
				S(H),
				S(I)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 0, "");
static_assert(FSM::stateId<B>() == 1, "");
static_assert(FSM::stateId<C>() == 2, "");
static_assert(FSM::stateId<D>() == 3, "");
static_assert(FSM::stateId<E>() == 4, "");
static_assert(FSM::stateId<F>() == 5, "");
static_assert(FSM::stateId<G>() == 6, "");
static_assert(FSM::stateId<H>() == 7, "");
static_assert(FSM::stateId<I>() == 8, "");

//------------------------------------------------------------------------------

struct A : FSM::State {};
struct B : FSM::State {};
struct C : FSM::State {};
struct D : FSM::State {};
struct E : FSM::State {};
struct F : FSM::State {};
struct G : FSM::State {};
struct H : FSM::State {};
struct I : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

void step1(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	server::FSM::Instance::SerialBuffer buffer;
	authority .save(buffer);
	replicated.load(buffer);

	logger.assertSequence({});

	REQUIRE(authority .isActive() == false);
	REQUIRE(replicated.isActive() == false);
}

//------------------------------------------------------------------------------

void step2(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	authority.enter();

	server::FSM::Instance::SerialBuffer serverBuffer;
	authority .save(serverBuffer);
	replicated.load(serverBuffer);

	logger.assertSequence({
		{ ffsm2::INVALID_STATE_ID,	 			Event::Type::ENTER },
		{ client::FSM::stateId<client::A>(),	Event::Type::ENTER },
	});

	REQUIRE(authority .activeStateId() == server::FSM::stateId<server::A>());
	REQUIRE(replicated.activeStateId() == client::FSM::stateId<client::A>());
}

//------------------------------------------------------------------------------

void step3(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated)
{
	server::FSM::Instance::SerialBuffer serverBuffer;
	server::FSM::Instance::SerialBuffer clientBuffer;

	authority .save(serverBuffer);
	replicated.save(clientBuffer);

	REQUIRE		 (clientBuffer == serverBuffer);
	REQUIRE_FALSE(clientBuffer != serverBuffer);
}

//------------------------------------------------------------------------------

void step4(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	authority.immediateChangeTo<server::C>();

	server::FSM::Instance::SerialBuffer buffer;
	authority .save(buffer);

	replicated.load(buffer);

	logger.assertSequence({
		{ client::FSM::stateId<client::A>(),	Event::Type::EXIT },

		{ client::FSM::stateId<client::C>(),	Event::Type::ENTER },
	});

	REQUIRE(authority .activeStateId() == server::FSM::stateId<server::C>());
	REQUIRE(replicated.activeStateId() == server::FSM::stateId<server::C>());
}

//------------------------------------------------------------------------------

void step5(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	authority.exit();

	server::FSM::Instance::SerialBuffer buffer;
	authority .save(buffer);

	replicated.load(buffer);

	logger.assertSequence({
		{ client::FSM::stateId<client::C>(),	Event::Type::EXIT },
		{ ffsm2::INVALID_STATE_ID,	 			Event::Type::EXIT },
	});

	REQUIRE(authority .isActive() == false);
	REQUIRE(replicated.isActive() == false);
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Serialization") {
	Logger logger;

	{
		server::FSM::Instance authority;
		client::FSM::Instance replicated{&logger};

		step1(authority, replicated, logger);
		step2(authority, replicated, logger);
		step3(authority, replicated);
		step4(authority, replicated, logger);
		step5(authority, replicated, logger);
	}

	logger.assertSequence({});
}

////////////////////////////////////////////////////////////////////////////////

}
