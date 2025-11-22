#include "Rocket.h"
#include <algorithm>
#include <sstream>
#include <cmath>

Rocket::Rocket(std::vector<Stage> stages, double cross_section_area)
    : stages_(std::move(stages)), area_(cross_section_area) {
    // compute total mass
    double total = 0.0;
    for (const auto &s : stages_) {
        total += s.dry_mass + s.prop_mass;
    }
    state_.time = 0.0;
    state_.altitude = 0.0;
    state_.velocity = 0.0;
    state_.mass = total;
    state_.stage_index = 0;
    if (!stages_.empty()) stages_[0].active = true;
}

double Rocket::drag(double v) const {
    // very simplified quadratic drag: 0.5 * rho * Cd * A * v^2
    // approximate rho as exp(-altitude/8500) * 1.225
    double rho0 = 1.225;
    double rho = rho0 * std::exp(-state_.altitude / 8500.0);
    double Cd = 0.5;
    return 0.5 * rho * Cd * area_ * v * std::abs(v);
}

void Rocket::stage_separation_if_needed() {
    if (state_.stage_index >= static_cast<int>(stages_.size())) return;
    auto &s = stages_[state_.stage_index];
    if (s.prop_mass <= 0.0 && s.active) {
        // separate stage: subtract dry mass and mark inactive
        state_.mass -= s.dry_mass;
        s.active = false;
        state_.stage_index++;
        if (state_.stage_index < static_cast<int>(stages_.size()))
            stages_[state_.stage_index].active = true;
    }
}

void Rocket::step(double dt, double throttle) {
    // clamp throttle
    throttle = std::clamp(throttle, 0.0, 1.0);
    // fetch active stage
    if (state_.stage_index >= static_cast<int>(stages_.size())) {
        // no stages left: rocket is coasting
        double acc = -g0 - drag(state_.velocity) / state_.mass;
        state_.velocity += acc * dt;
        state_.altitude += state_.velocity * dt;
        state_.time += dt;
        return;
    }
    Stage &s = stages_[state_.stage_index];
    // current thrust scaled by throttle and reduced if fuel gone
    double available_prop = s.prop_mass;
    double mass_flow = 0.0;
    double thrust = 0.0;
    if (available_prop > 1e-6 && s.thrust > 0.0) {
        // approximate mass flow = thrust / (Isp * g0)
        double mdot_nom = s.thrust / (s.isp * g0);
        mass_flow = mdot_nom * throttle;
        // limit mass flow to remaining prop mass / dt
        double max_possible = available_prop / dt;
        if (mass_flow > max_possible) mass_flow = max_possible;
        thrust = mass_flow * s.isp * g0; // back-calc thrust to be consistent
        // burn fuel
        s.prop_mass -= mass_flow * dt;
        state_.mass -= mass_flow * dt;
    }
    // forces
    double weight = state_.mass * g0;
    double Fdrag = drag(state_.velocity);
    double netF = thrust - weight - Fdrag;
    double accel = netF / state_.mass;
    // integrate (simple Euler)
    state_.velocity += accel * dt;
    state_.altitude += state_.velocity * dt;
    state_.time += dt;
    // stage separation
    if (s.prop_mass <= 0.0) {
        stage_separation_if_needed();
    }
    // prevent negative altitude below ground
    if (state_.altitude < 0.0) {
        state_.altitude = 0.0;
        state_.velocity = 0.0;
    }
}

const State& Rocket::get_state() const { return state_; }
double Rocket::get_total_mass() const { return state_.mass; }
bool Rocket::finished() const {
    // finished when no stages active and vertical speed is low and prop mass 0
    bool any_active = false;
    for (auto &s: stages_) if (s.active) any_active = true;
    return !any_active;
}
