cmake_minimum_required(VERSION 3.20)
project(asm-interpet-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

find_package(Boost REQUIRED program_options)

include_directories("include" ${Boost_INCLUDE_DIRS})

add_executable(assembler
    main_asm.cpp
    src/Assembler.cpp
    src/Common.cpp
)
target_link_libraries(assembler PRIVATE Boost::boost Boost::program_options)

add_executable(interpreter
    main_inter.cpp
    src/Interpreter.cpp
    src/Common.cpp
)
target_link_libraries(interpreter PRIVATE Boost::boost Boost::program_options)

enable_testing()
add_subdirectory(tests)