from setuptools import setup, Extension
import pybind11

cpp_args = ['-std=c++11', '-fvisibility=hidden']  # Added visibility flag

ext_modules = [
    Extension(
        "gymcar",
        ["gymcar.cpp"],
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=cpp_args,
    ),
]

setup(
    name="gymcar",
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.4.3', 'gymnasium'],
    setup_requires=['pybind11>=2.4.3'],
    python_requires=">=3.7",
)
