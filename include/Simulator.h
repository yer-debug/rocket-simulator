#pragma once
#include "Rocket.h"
#include "Logger.h"
#include <functional>

class Simulator {
public:
    Simulator(Rocket rocket, double dt = 0.1);
    // run for max_time seconds, guidance function returns throttle [0..1]
    void run(double max_time, std::function<double(const State&)> guidance, Logger &logger);
private:
    Rocket rocket_;
    double dt_;
};
