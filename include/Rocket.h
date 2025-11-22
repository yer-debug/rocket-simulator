#pragma once
#include "Types.h"

struct Stage {
    double dry_mass;      // kg
    double prop_mass;     // kg
    double thrust;        // N (nominal)
    double isp;           // s (specific impulse)
    bool active;
};

struct State {
    double time;          // seconds
    double altitude;      // meters
    double velocity;      // m/s (up positive)
    double mass;          // kg (total)
    int stage_index;      // current stage
};

class Rocket {
public:
    Rocket(std::vector<Stage> stages, double cross_section_area = 10.0);
    // integrate one timestep given throttle [0..1]
    void step(double dt, double throttle);
    const State& get_state() const;
    double get_total_mass() const;
    bool finished() const; // all stages exhausted or crashed/escaped

private:
    std::vector<Stage> stages_;
    State state_;
    double area_; // m^2, aerodynamic reference
    static constexpr double g0 = 9.80665;
    double drag(double v) const;
    void stage_separation_if_needed();
};
