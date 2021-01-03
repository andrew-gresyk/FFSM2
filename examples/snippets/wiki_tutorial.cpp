// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#include <catch2/catch.hpp>
#undef assert
#define assert(x) REQUIRE(x)

// Configure optional FFSM2 functionality using #defines
// (in this case we're using Plans to make transition cycle more straightforward):
#define FFSM2_ENABLE_PLANS

// Include FFSM2 header:
#include <ffsm2/machine.hpp>

////////////////////////////////////////////////////////////////////////////////

// Define interface class between the state machine and its host
// (also ok to use the host object itself):
struct Context {
    bool powerOn;
};

// (Optional) Define type config:
using Config = ffsm2::Config::ContextT<Context>;

// (Optional, recommended) Define ffsm2::Machine for convenience:
using M = ffsm2::MachineT<Config>;

//------------------------------------------------------------------------------

// Declare state machine structure.
// States need to be forward declared, e.g. with a magic macro:
#define S(s) struct s

using FSM = M::Root<S(On),
                S(Off),                                // initial top-level state
                S(Red),					               // initial sub-state of the region
                S(Yellow),
                S(Green),
                S(Done)
            >;

#undef S

//------------------------------------------------------------------------------

// While FFSM2 transitions aren't event-based,
// events can be used to have FSM react to external stimuli:
struct Event {};

//------------------------------------------------------------------------------

// Define states and override required state methods:
struct Off
    : FSM::State
{
    void entryGuard(FullControl& control) {            // called before state activation, use to re-route transitions
        if (control.context().powerOn)                 // access shared data
            control.changeTo<Red>();                   // initiate a transition into 'On' region
    }

    void exit(PlanControl& /*control*/) {}             // called on state deactivation
};

struct On
    : FSM::State
{
    void enter(PlanControl& control) {                 // called on state activation
        auto plan = control.plan();                    // access the plan for the region

        plan.change<Red, Yellow>();                    // sequence plan steps, executed when the previous state succeeds
        plan.change<Yellow, Green>();
        plan.change<Green, Yellow>();
        plan.change<Yellow, Red>();
    }

    void exit(PlanControl& /*control*/) {}             // called on state deactivation

    void planSucceeded(FullControl& control) {         // called on the successful completion of all plan steps
        control.changeTo<Done>();
    }

    void planFailed(FullControl& /*control*/) {}       // called if any of the plan steps fails
};

struct Red
    : FSM::State
{
    void update(FullControl& control) {                // called on periodic state machine updates
        control.succeed();                             // notify successful completion of the plan step
    }                                                  // plan will advance to the 'Yellow' state
};

struct Yellow
    : FSM::State
{
    void update(FullControl& control) {
        control.succeed();                             // plan will advance to the 'Green' state on the first entry
                                                       // and 'Red' state on the second one
    }
};

struct Green
    : FSM::State
{
    void react(const Event&, FullControl& control) {   // called on external events
        control.succeed();                             // advance to the next plan step
    }
};

struct Done
    : FSM::State
{};

//------------------------------------------------------------------------------

// Write the client code to use your new state machine:
TEST_CASE("Wiki.Tutorial", "[Wiki]") {
    // Create context and state machine instances:
    Context context;
    context.powerOn = true;

    FSM::Instance fsm{context};
    assert(fsm.isActive<Red>());                       // On's initial sub-state

    fsm.update();
    assert(fsm.isActive<Yellow>());                    // 1st setp of On's plan

    fsm.update();
    assert(fsm.isActive<Green>());                     // 2nd setp of On's plan

    fsm.react(Event{});
    assert(fsm.isActive<Yellow>());                    // 3rd setp of On's plan

    fsm.update();
    assert(fsm.isActive<Red>());                       // 4th setp of On's plan

    fsm.update();
    assert(fsm.isActive<Done>());                      // activated by On::planSucceeded()
}

////////////////////////////////////////////////////////////////////////////////
