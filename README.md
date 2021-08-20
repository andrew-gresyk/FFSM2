[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![GitHub Release Date](https://img.shields.io/github/release-date/andrew-gresyk/FFSM2)](https://github.com/andrew-gresyk/FFSM2/releases)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/andrew-gresyk/FFSM2.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/andrew-gresyk/FFSM2/context:cpp)  
[![MSBuild](https://github.com/andrew-gresyk/FFSM2/actions/workflows/msbuild.yml/badge.svg)](https://github.com/andrew-gresyk/FFSM2/actions/workflows/msbuild.yml)
[![CMake](https://github.com/andrew-gresyk/FFSM2/actions/workflows/cmake.yml/badge.svg)](https://github.com/andrew-gresyk/FFSM2/actions/workflows/cmake.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/io1x70fd52l04wad?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/FFSM2)  
[![Gitter](https://badges.gitter.im/andrew-gresyk/FFSM2.svg)](https://gitter.im/andrew-gresyk/FFSM2)
[![Discord](https://img.shields.io/discord/591914197219016707.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/ujpNbqSpfm)
[![Twitter Follow](https://img.shields.io/twitter/follow/andrew_gresyk?style=social)](https://www.twitter.com/andrew_gresyk)

---

![FFSM2](assets/logos/ffsm2-logo-large.png)

# High-Performance Flat Finite State Machine

Header-only flat FSM framework in C++11, with fully statically-defined structure (no dynamic allocations), built with variadic templates.

---

## Compiler Support

- Visual Studio 14, 15, 16
- GCC 5, 6, 7, 8, 9, 10
- Clang 3.7, 3.8, 3.9, 4, 5, 6, 7, 8, 9, 10, 11

---

## Quick Start

- **[Tutorial](examples/snippets/wiki_tutorial.cpp)**

---

## See Also

- **[FFSM2](https://hfsm.dev)**: High-Performance **Hierarchical** Finite State Machine

---

## Documentation

- GitBook: **[flatdoc.hfsm.dev](https://flatdoc.hfsm.dev/)** (WIP)
- In-line comments

---

## Feature Highlights

- Permissive **[MIT License](LICENSE)**
- Written in widely-supported modern(ish) C++11
- Header-only
- Convenient, minimal boilerplate
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time polymorphism, no virtual methods are used
- Type-safe transitions: `FSM.changeTo<TargetState>()` with optional payloads
- Scaleable, supports robust state re-use via state injections
- Gamedev-friendly, supports explicit `State::update()`
- Also supports traditional event-based workflow with `State::react()`
- Inspect anything: previous and current transitions, state activation status, and more!
- Game AI-friendly with dynamic planning support
- Built-in logging support

---

## 3rd Party Libraries

- **[doctest](https://github.com/onqtam/doctest)** unit testing framework

---

## Get In Touch

- Twitter: **[@andrew_gresyk](https://www.twitter.com/andrew_gresyk)**
- Discord: **[HFSM.dev](https://discord.gg/v4t3tzh)**
- Gitter: **[andrew-gresyk/FFSM2](https://gitter.im/andrew-gresyk/FFSM2)**
