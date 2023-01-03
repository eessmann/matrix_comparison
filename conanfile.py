from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class MatrixComparision(ConanFile):
    # Note: options are copied from CMake boolean options.
    # When turned off, CMake sometimes passes them as empty strings.
    name = "BioFM"
    version = "0.1"
    settings = "os", "arch", "compiler", "build_type"
    requires = (  "armadillo/10.7.3"
                , "blaze/3.8"
                , "cli11/2.3.1"
                , "catch2/3.2.1"
                , "eigen/3.4.0"
                , "spdlog/1.11.0"
                , "gsl/2.7"
                , "pcg-cpp/cci.20210406"
                , "xtensor/0.24.2")
    default_options = {
    }


    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
