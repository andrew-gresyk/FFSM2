#include <ffsm/machine.hpp>

namespace test {

struct Context {};
using M = ffsm::Machine<Context>;

// forward declare states for transitions below
struct YellowUp;
struct YellowDown;
struct Green;

// optional, can be useful to delay core functionality if required
struct Off
	: M::State
{};

// normal traffic light states
struct Red
	: M::State
{
	virtual void update(Control& control) override {
		control.changeTo<YellowDown>();
	}
};

struct YellowUp
	: M::State
{
	virtual void update(Control& control) override {
		control.changeTo<Red>();
	}
};

struct YellowDown
	: M::State
{
	virtual void update(Control& control) override {
		control.changeTo<Green>();
	}
};

struct Green
	: M::State
{
	virtual void update(Control& control) override {
		control.changeTo<YellowUp>();
	}
};

// FSM structure declaration
using FSM = M::Host<Off,
					Red,
					YellowUp,
					YellowDown,
					Green>;
}
