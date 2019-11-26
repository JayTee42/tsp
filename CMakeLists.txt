cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(TSP VERSION 1.0 LANGUAGES C)

find_package(OpenMP REQUIRED)

add_executable(tsp tsp.c)
target_compile_definitions(tsp PUBLIC USE_CTZ_MACRO)
target_link_libraries(tsp PUBLIC OpenMP::OpenMP_C)

install(
    TARGETS
        tsp
    RUNTIME DESTINATION bin
)