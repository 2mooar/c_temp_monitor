cmake_minimum_required(VERSION 3.20)
project(temperature_monitor_system C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)

# Options
option(ENABLE_WARNINGS "Enable strict warnings" ON)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(USE_MOCK_HAL "Build with mock HAL for host/testing" ON)

if(ENABLE_WARNINGS)
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
endif()

if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()



# Targets
# Todo split CORE into HAL and CORE for better portability
add_library(core
    sources/temperature_monitor.c
    sources/drivers/temperature_sensor.c
    sources/drivers/led.c
    sources/drivers/ring_buffer.c
    sources/drivers/eeprom.c
)

if (USE_MOCK_HAL)
  message(STATUS "Building with MOCK HAL")
  target_sources(core PRIVATE sources/hal/mock_hal.c)
  target_compile_definitions(core PUBLIC USE_MOCK_HAL=1)
else()
  message(STATUS "Building with REAL HAL stubs")
  target_sources(core PRIVATE sources/hal/real_hal.c)
  target_compile_definitions(core PUBLIC USE_MOCK_HAL=0)
endif()

target_include_directories(core PUBLIC includes)
target_include_directories(core PUBLIC includes/hal)
target_include_directories(core PUBLIC includes/drivers)

add_executable(app sources/main.c)

target_link_libraries(app PRIVATE core)

