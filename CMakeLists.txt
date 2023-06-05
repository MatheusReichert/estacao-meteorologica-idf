# For more information about the build system, see:
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html

# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly.
cmake_minimum_required(VERSION 3.16)
include(/home/reichert/esp/esp-idf/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS
    main
    $ENV{HOME}/esp/esp-idf-lib/components
)
project(weather-station)
