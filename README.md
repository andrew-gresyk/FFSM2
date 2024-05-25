[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![GitHub Release Date](https://img.shields.io/github/release-date/andrew-gresyk/FFSM2)](https://github.com/andrew-gresyk/FFSM2/releases)  
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/4617/badge)](https://bestpractices.coreinfrastructure.org/projects/4617)  
[![GCC, Clang](https://github.com/andrew-gresyk/FFSM2/actions/workflows/cmake.yml/badge.svg)](https://github.com/andrew-gresyk/FFSM2/actions/workflows/cmake.yml)
[![ARM GCC](https://github.com/andrew-gresyk/FFSM2/actions/workflows/cmake-qemu-arm.yml/badge.svg)](https://github.com/andrew-gresyk/FFSM2/actions/workflows/cmake-qemu-arm.yml)
[![MS VS](https://github.com/andrew-gresyk/FFSM2/actions/workflows/msbuild.yml/badge.svg)](https://github.com/andrew-gresyk/FFSM2/actions/workflows/msbuild.yml)  
[![Gitter](https://badges.gitter.im/andrew-gresyk/FFSM2.svg)](https://gitter.im/andrew-gresyk/FFSM2)
[![Discord](https://img.shields.io/discord/755015945269018695.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/ZfWjhFj)
[![Twitter Follow](https://img.shields.io/twitter/follow/andrew_gresyk?style=social)](https://www.twitter.com/andrew_gresyk)

---

![FFSM2](assets/logos/ffsm2-logo-large.png)

# High-Performance Flat Finite State Machine

Header-only flat FSM framework in C++11, with fully statically-defined structure (no dynamic allocations), built with variadic templates.

---

## Compiler Support

- Visual Studio: **2017, 2019, 2022**
- GCC: **8, 9, 10, 11, 12, 13, 14**
- GCC (ARM): **9**
- Clang: **6, 7, 8, 9, 10, 11, 12, 13, 14, 15**
- AppleClang: **14**
- AppleClang (ARM): **14**

---

## Quick Start

- **[Minimal Example](https://github.com/andrew-gresyk/FFSM2/wiki/Overview-Minimal-Example)**
- **[Extended Tutorial](https://github.com/andrew-gresyk/FFSM2/wiki/Overview-Extended-Tutorial)**

---

## See Also

- **[FFSM2 UPlugin](https://github.com/andrew-gresyk/FFSM2-uplugin)**: Unreal Engine plugin for [FFSM2](https://flat.hfsm.dev) library
- **[HFSM2](https://hfsm.dev)**: High-Performance **Hierarchical** Finite State Machine
- **[HFSM2 UPlugin](https://github.com/andrew-gresyk/HFSM2-uplugin)**: Unreal Engine plugin for [HFSM2](https://hfsm.dev) library
- **[HFSM Mono](https://font.hfsm.dev/)**: Monospaced bitmap Unicode fonts for HFSM2 and FFSM2 libraries

---

## Documentation

- **[Wiki](https://github.com/andrew-gresyk/FFSM2/wiki)**

---

## Feature Highlights

- Permissive **[MIT License](LICENSE)**
- Written in widely-supported modern(ish) C++11
- Header-only
- Convenient, minimal boilerplate
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time polymorphism, no virtual methods are used
- Type-safe transitions: `FSM.changeTo<TargetState>()` with optional payloads
- Flexible configuration using `FFSM2_ENABLE_*` macros
- Scalable, supports robust state re-use via state injections
- Gamedev-friendly, supports explicit `State::update()`
- Also supports traditional event-based workflow with `State::react()`
- Inspect anything: previous and current transitions, state activation status, and more!
- Game AI-friendly with dynamic planning support
- **[Debug-assisted](https://gresyk.dev/features/2018/01/15/hfsm-magic.html)**, includes automatic structure and activity visualization API
- Built-in logging support

---

## 3rd Party Libraries

- **[doctest](https://github.com/onqtam/doctest)** unit testing framework

---

## Get In Touch

- Twitter: **[@andrew_gresyk](https://www.twitter.com/andrew_gresyk)**
- Discord: **[HFSM.dev](https://discord.gg/ZfWjhFj)**
- Gitter: **[andrew-gresyk/FFSM2](https://gitter.im/andrew-gresyk/FFSM2)**

---

## Special Thanks

- [Alexis Paques](https://github.com/AlexisTM)
