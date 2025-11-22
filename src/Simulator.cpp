#include "Simulator.h"
#include <sstream>
#include <iomanip>

Simulator::Simulator(Rocket rocket, double dt) : rocket_(std::move(rocket)), dt_(dt) {}

void Simulator::run(double max_time, std::function<double(const State&)> guidance, Logger &logger) {
    double t = 0.0;
    while (t < max_time) {
        const State &s = rocket_.get_state();
        // guidance returns throttle between 0 and 1
        double throttle = guidance(s);
        rocket_.step(dt_, throttle);

        // log
        const State &ns = rocket_.get_state();
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2)
           << ns.time << "," << ns.altitude << "," << ns.velocity << "," << ns.mass << "," << ns.stage_index;
        logger.log_line(ss.str());

        if (rocket_.finished()) break;
        t += dt_;
    }
}
