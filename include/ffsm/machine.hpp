// FFSM (Flat Finite State Machine)
//		See https://github.com/andrew-gresyk/HFSM2 for details
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2019
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#ifdef __GNUC__
#include <utility>		// @GCC: std::conditional<>, move(), forward()
#endif

namespace ffsm {

// 'templated namespace' for propagating TContext throughout the FSM
template <typename TContext>
struct Machine {
	using Context = TContext;

	// encapsulate all control functions within one utility class
	class Control {
	public:
		Control(Context& context)
			: _context{context}
			, _requested{typeid(void)}
		{}

		// initiate delayed transition from within the state
		template <typename TState>
		void changeTo() {
			_requested = typeid(TState);
		}

		auto& context() {
			return _context;
		}

		auto requested() const {
			return _requested;
		}

	private:
		Context& _context;
		std::type_index _requested;
	};

	// inheritable state base class with empty methods to override
	class State {
	protected:
		using Control = Machine::Control;

	public:
		virtual ~State() {}

		virtual void enter (Context&) {}
		virtual void update(Control&) {}
		virtual void exit  (Context&) {}
	};

	// the main FSM class
	template <typename... TStates>
	class Host {
		using States = std::unordered_map<std::type_index, std::unique_ptr<State>>;

		// helper template to chop off the first type
		template <typename T, typename...>
		struct First {
			using Type = T;
		};
		using Initial = typename First<TStates...>::Type;

		template <typename T>
		void initializationHelper() {
			_states[std::type_index{typeid(T)}] = std::move(std::unique_ptr<State>{new T});
		}

	public:
		Host(Context& context)
			: _context{context}
			, _states{sizeof...(TStates)}
		{
			(initializationHelper<TStates>(), ...);

			_active = _states.find(typeid(Initial))->second.get();
			_active->enter(_context);
		}

		~Host() {
			_active->exit(_context);
		}

		// main update method, which in turn updates the current state and handles internal transitions
		void update() {
			Control control{_context};
			_active->update(control);

			if (control.requested() != typeid(void)) {
				_active->exit(_context);
				_active = _states.find(control.requested())->second.get();
				_active->enter(_context);
			}
		}

		// perform immediate transition
		template <typename TState>
		void changeTo() {
			_active->exit(_context);
			_active = _states.find(typeid(TState))->second.get();
			_active->enter(_context);
		}

		// basic introspection
		template <typename TState>
		bool isActive() const {
			return _active == _states.find(typeid(TState))->second.get();
		}

	private:
		Context& _context;
		States _states;
		State* _active = nullptr;
	};
};

}
