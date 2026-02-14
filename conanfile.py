from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class FlockConan(ConanFile):
    name = "flock"
    version = "0.1"

    author = "Mateo Cervilla"
    url = "https://github.com/mateocervilla/Flock"
    description = "A simple C++ + SFML Flocking simulator"
    topics = ("flocking", "simulation", "sfml")

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    options = {
        "shared": [True, False],
    }

    default_options = {
        "shared": False,
    }

    def requirements(self):
        self.requires("sfml/2.6.1")

    def configure(self):
        self.options["sfml"].shared = self.options.shared

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
