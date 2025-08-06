# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/gaelenguzman/.platformio/packages/framework-espidf/components/bootloader/subproject")
  file(MAKE_DIRECTORY "/Users/gaelenguzman/.platformio/packages/framework-espidf/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader"
  "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix"
  "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix/tmp"
  "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix/src/bootloader-stamp"
  "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix/src"
  "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/gaelenguzman/Documents/PlatformIO/Projects/ESP32-S3_TestingGround/.esphome/build/test-i2c/.pioenvs/test-i2c/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
