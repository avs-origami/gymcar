#include <iostream>
#include <vector>
#include <pybind11/embed.h>
#include "gymcar.h"

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{};

    try {
        // Create environment with rendering enabled
        CarRacingEnv env(true);

        // Reset the environment
        auto observation = env.reset();

        // Run a few steps
        for (int i = 0; i < 100; i++) {
            // Sample action: [steering, gas, brake]
            std::vector<float> action = {0.0f, 0.5f, 0.0f};  // Go straight with half gas

            // Take a step
            auto [next_obs, reward, terminated, truncated, info] = env.step(action);

            // Render the environment
            env.render();

            // Check if episode is done
            if (terminated || truncated) {
                observation = env.reset();
            } else {
                observation = next_obs;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
