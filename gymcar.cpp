#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>
#include <string>

#include "gymcar.h"

namespace py = pybind11;

// Add these using declarations to handle the keyword arguments
using namespace pybind11::literals;

CarRacingEnv::CarRacingEnv(bool render) : render_mode_set(render) {
    try {
        // Import gymnasium
        gym_module = py::module::import("gymnasium");
        
        // Create a kwargs dictionary for the make function
        py::dict kwargs;
        if (render_mode_set) {
            kwargs["render_mode"] = "human";
        } else {
            kwargs["render_mode"] = "rgb_array";
        }
        
        // Create environment with appropriate render mode
        env = gym_module.attr("make")("CarRacing-v3", **kwargs);
    }
    catch (const py::error_already_set& e) {
        throw std::runtime_error("Failed to initialize Gymnasium environment: " + std::string(e.what()));
    }
}

CarRacingEnv::~CarRacingEnv() {
    if (!env.is_none()) {
        env.attr("close")();
    }
}

// Reset the environment
py::array_t<float> CarRacingEnv::reset() {
    try {
        py::tuple reset_result = env.attr("reset")();
        py::array_t<float> observation = reset_result[0].cast<py::array_t<float>>();
        return observation;
    }
    catch (const py::error_already_set& e) {
        throw std::runtime_error("Failed to reset environment: " + std::string(e.what()));
    }
}

// Step the environment
std::tuple<py::array_t<float>, float, bool, bool, py::dict> CarRacingEnv::step(const std::vector<float>& action) {
    try {
        // Convert C++ vector to Python list
        py::array_t<float> py_action(action.size());
        std::memcpy(py_action.mutable_data(), action.data(), action.size() * sizeof(float));

        // Take step in environment
        py::tuple step_result = env.attr("step")(py_action);

        // Parse return values
        py::array_t<float> observation = step_result[0].cast<py::array_t<float>>();
        float reward = step_result[1].cast<float>();
        bool terminated = step_result[2].cast<bool>();
        bool truncated = step_result[3].cast<bool>();
        py::dict info = step_result[4].cast<py::dict>();

        return std::make_tuple(observation, reward, terminated, truncated, info);
    }
    catch (const py::error_already_set& e) {
        throw std::runtime_error("Failed to step environment: " + std::string(e.what()));
    }
}

// Render the environment
void CarRacingEnv::render() {
    if (!render_mode_set) {
        throw std::runtime_error("Environment not initialized with render mode");
    }
    try {
        env.attr("render")();
    }
    catch (const py::error_already_set& e) {
        throw std::runtime_error("Failed to render environment: " + std::string(e.what()));
    }
}

PYBIND11_MODULE(car_racing_env, m) {
    py::class_<CarRacingEnv>(m, "CarRacingEnv")
        .def(py::init<bool>(), py::arg("render") = false)
        .def("reset", &CarRacingEnv::reset)
        .def("step", &CarRacingEnv::step)
        .def("render", &CarRacingEnv::render);
}
