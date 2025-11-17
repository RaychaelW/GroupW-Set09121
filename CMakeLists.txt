project(setup)
cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### Setup Directories ####
# Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all built files (executables & libs) into bin/
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")

#### Game Project ####
add_executable(game
    main.cpp
    game.cpp
)

# Include SFML headers
target_include_directories(game PRIVATE ${SFML_INCS})

# Link required SFML modules
target_link_libraries(game
    sfml-graphics
    sfml-window
    sfml-system
)