cmake_minimum_required(VERSION 3.18)

project(PitchDetectorTests LANGUAGES CXX)

add_executable(PitchDetectorTests
    PitchDetectorTest.cpp
    ../Source/PitchDetector.cpp
)

target_compile_features(PitchDetectorTests PRIVATE cxx_std_17)

