#include <iostream>
#include "../include/Rocket.h"
#include "../include/Simulator.h"
#include "../include/Logger.h"

int main() {
    std::vector<Stage> stages = {
        {1000.0, 5000.0, 1e5, 250.0, true}
    };
    Rocket r(stages);
    Logger logger("test_telemetry.csv");
    Simulator sim(r, 0.1);
    sim.run(20.0, [](const State &s){ return 1.0; }, logger);
    std::cout << "Test run complete. See test_telemetry.csv\n";
    return 0;
}
