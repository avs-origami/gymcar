#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>
#include <string>
#include <tuple>

namespace py = pybind11;

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

class EXPORT CarRacingEnv {
public:
    CarRacingEnv(bool render = false);
    ~CarRacingEnv();

    py::array_t<float> reset();
    std::tuple<py::array_t<float>, float, bool, bool, py::dict> step(const std::vector<float>& action);
    void render();

private:
    py::object gym_module;
    py::object env;
    bool render_mode_set;
};
