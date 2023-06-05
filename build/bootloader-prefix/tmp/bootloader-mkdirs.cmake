# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/reichert/esp/esp-idf/components/bootloader/subproject"
  "/home/reichert/pi2/weather-station/build/bootloader"
  "/home/reichert/pi2/weather-station/build/bootloader-prefix"
  "/home/reichert/pi2/weather-station/build/bootloader-prefix/tmp"
  "/home/reichert/pi2/weather-station/build/bootloader-prefix/src/bootloader-stamp"
  "/home/reichert/pi2/weather-station/build/bootloader-prefix/src"
  "/home/reichert/pi2/weather-station/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/reichert/pi2/weather-station/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/reichert/pi2/weather-station/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
