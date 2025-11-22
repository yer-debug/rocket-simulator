#include "Types.h"
#include "Rocket.h"
#include "Simulator.h"
#include "Logger.h"
#include <iostream>
#include <cmath>

// A very simple guidance: attempt to follow a vertical velocity profile.
// This is intentionally simple and non-operational.
double simple_guidance(const State &s) {
    // target: during stage 0: accelerate to 300 m/s, during later: coast
    double desired_v;
    if (s.stage_index == 0) desired_v = 300.0;
    else if (s.stage_index == 1) desired_v = 450.0;
    else desired_v = 0.0;

    double error = desired_v - s.velocity;
    // proportional controller (just for simulation)
    double Kp = 0.002; // small gain
    double throttle = Kp * error;
    // bias: ensure some throttle at liftoff
    if (s.time < 2.0) throttle = std::max(throttle, 0.8);
    return std::clamp(throttle, 0.0, 1.0);
}

int main() {
    try {
        // define two-stage rocket (numbers are arbitrary and fictional)
        std::vector<Stage> stages = {
            // first stage: large thrust, lots of propellant
            { 20000.0,  300000.0,  9.0e6, 300.0, true },
            // second stage: smaller thrust, high Isp
            { 4000.0,   80000.0,   1.2e6, 350.0, false }
        };
        Rocket rocket(stages, 12.0);
        Simulator sim(rocket, 0.1);
        Logger logger("telemetry.csv");

        sim.run(400.0, simple_guidance, logger);

        std::cout << "Simulation finished. Telemetry written to telemetry.csv\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }
    return 0;
}
