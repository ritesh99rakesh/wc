cmake_minimum_required(VERSION 3.30)

include(cmake/prelude.cmake)

project(
    wc
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "''"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    wc_lib OBJECT
    src/lib.cpp
)

target_include_directories(
    wc_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

target_compile_features(wc_lib PUBLIC cxx_std_23)

find_package(Boost COMPONENTS program_options REQUIRED)

# ---- Declare executable ----

add_executable(wc_exe src/main.cpp)
add_executable(wc::exe ALIAS wc_exe)

set_property(TARGET wc_exe PROPERTY OUTPUT_NAME wc)

target_compile_features(wc_exe PRIVATE cxx_std_23)

target_link_libraries(wc_exe PRIVATE wc_lib)

target_link_libraries(wc_exe PRIVATE Boost::program_options)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT wc_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of wc"
  )
endif()

include(cmake/dev-mode.cmake)
