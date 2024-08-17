# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(wcTests LANGUAGES CXX)

# ---- Tests ----

add_executable(wc_test src/wc_test.cpp)
target_link_libraries(wc_test PRIVATE wc_lib)
target_compile_features(wc_test PRIVATE cxx_std_23)

add_test(NAME wc_test COMMAND wc_test)

# ---- End-of-file commands ----

add_folders(Test)
