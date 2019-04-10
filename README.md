[![Standard](https://img.shields.io/badge/c%2B%2B-14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/gy4gu98j2n3phtcx?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/ffsm)
[![Build Status](https://travis-ci.org/andrew-gresyk/FFSM.svg?branch=master)](https://travis-ci.org/andrew-gresyk/FFSM)

# FFSM: Flat Finite State Machine Framework

Header-only flat FSM framework, made as an example to illustrate [twitter thread on #cpp #state_machine #software_design](https://threadreaderapp.com/thread/1108411688079015936.html).

## Compiler Support

- Visual Studio 14, 15, 16
- GCC 7, 8
- Clang 5, 6, 7

---

## Basic Usage

1. Include FFSM header:

```cpp
#include <ffsm/machine.hpp>
```

2. Define interface class between the state machine and its host
(also ok to use the host object itself):

```cpp
struct Context {
    // ...
};
```

3. (Optional, recommended) Typedef `ffsm::Machine` for convenience:

```cpp
using M = ffsm::Machine<Context>;
```

4. (Optional) Forward declare transition target states:

```cpp
struct Second;
```

5. Define states inheriting them from `M::State`:

```cpp
struct First
    : M::State
{
    virtual void update(Control& control) override {
        control.changeTo<Second>();
    }
};

struct Second
    : M::State
{
    virtual void update(Control& control) override {
        control.changeTo<First>();
    }
};

struct Done : M::State {};

```

6. Declare state machine structure:

```cpp
using FSM = M::Host<First,
                    Second,
                    Done>;
```

7. Write the client code to use your new state machine:

```cpp
int main() {
```

8. Create context and state machine instances:

```cpp
    Context context;
    FSM fsm(context);
```

9. Kick off periodic updates until it's done:

```cpp
    while (!fsm.isActive<Done>())
        fsm.update();

    return 0;
}
```

## Get Updates

- [Original twitter thread on #cpp #state_machine #software_design](https://threadreaderapp.com/thread/1108411688079015936.html)
- [FFSM's "Older sibling" - Hierarchical FSM framework](https://hfsm.dev)
- [Blog](https://gresyk.dev)
- [Twitter](https://www.twitter.com/andrew_gresyk)
