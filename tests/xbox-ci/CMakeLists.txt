cmake_minimum_required(VERSION 3.24)
project(veyra_xbox_ci LANGUAGES C CXX)

if(NOT WIN32)
  message(FATAL_ERROR "The Xbox native transport is Windows-only")
endif()

get_filename_component(VEYRA_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
list(APPEND CMAKE_MODULE_PATH "${VEYRA_ROOT}/cmake")
include(VeyraWarnings)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# The focused target compiles the real Xbox HTTPS/session/WebRTC sources. A
# no-I/O log shim avoids pulling the unrelated media, GPU and diagnostics graph
# into this CI job.
add_library(veyra_base STATIC LogShim.cpp)
target_include_directories(veyra_base PUBLIC "${VEYRA_ROOT}/include")
target_compile_definitions(veyra_base PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
target_compile_features(veyra_base PUBLIC cxx_std_20)

include(VeyraXbox)

add_executable(veyra_fsr_provider_selection_tests FsrProviderSelectionTests.cpp)
target_include_directories(veyra_fsr_provider_selection_tests PRIVATE "${VEYRA_ROOT}/include")
target_compile_features(veyra_fsr_provider_selection_tests PRIVATE cxx_std_20)
veyra_apply_warnings(veyra_fsr_provider_selection_tests)

enable_testing()
add_test(NAME xbox-native-protocol COMMAND veyra_xbox_tests)
add_test(NAME fsr-provider-selection COMMAND veyra_fsr_provider_selection_tests)
