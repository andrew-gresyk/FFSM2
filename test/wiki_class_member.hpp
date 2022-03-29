// FFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#pragma once

class Actor {
public:
	using FsmHost = char[32]; // the size is hand-adjusted

	struct Context { /* .. */ };

public:
	Actor();
	~Actor();

private:
	Context _context;
	FsmHost _fsmHost;
};
