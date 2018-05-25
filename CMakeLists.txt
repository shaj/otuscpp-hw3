cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "0")
if($ENV{TRAVIS_BUILD_NUMBER})
  set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
endif()


project(allocator VERSION 0.0.${PATCH_VERSION})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Threads)

add_executable(allocator main.cpp newdelete.cpp)

add_executable(test_allocator test_allocator.cpp newdelete.cpp)

target_compile_definitions(allocator PRIVATE SPDLOG_TRACE_ON)

set_target_properties(allocator test_allocator PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(allocator
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(test_allocator PROPERTIES
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)


target_link_libraries(test_allocator
  ${Boost_LIBRARIES}
)


configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(allocator PRIVATE "${PROJECT_SOURCE_DIR}")
target_include_directories(allocator PRIVATE "${PROJECT_BINARY_DIR}")


install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kolobrod2002@yandex.ru)

include (CPack)

enable_testing()

add_test(test_allocator test_allocator)
