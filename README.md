# Starship-like Educational Simulator (C++)

## What this is
A simplified, educational 2D vertical rocket simulator implemented in modern C++20.
It models multi-stage rockets, simple aerodynamics (quadratic drag), mass depletion due to propellant burn,
and a trivial guidance loop for throttle. The goal is learning and experimentation.

**This software is NOT for any real-world spacecraft guidance or control.**

## Files
- `include/` : public headers
- `src/`     : implementation
- `tests/`   : tiny test harness
- `CMakeLists.txt` : build config

## Build & Run
```bash
mkdir build
cd build
cmake ..
make -j
./starship_sim
# or run tests
./starship_test

# rocket-simulator
Advanced C++ rocket simulation with trajectory modeling and AI-ready architecture
>>>>>>> 7e3eec499c64a44424c1df34f13707836c7f4559
