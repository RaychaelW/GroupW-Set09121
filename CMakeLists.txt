project(the_quest)
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

file(COPY resources/textures DESTINATION ${CMAKE_BINARY_DIR})

#### Game Project ####
add_executable(the_quest
        src/main.cpp
        src/game.cpp
        src/state.hpp
        src/stateManager.hpp
        src/stateManager.cpp
        src/mainMenuState.cpp
        src/mainMenuState.hpp
        src/gameState.cpp
        src/gameState.hpp
        src/InputManager.cpp
        src/InputManager.hpp
        src/Player.cpp
        src/Player.hpp
)

# Include SFML headers
target_include_directories(the_quest PRIVATE ${SFML_INCS})

# Link required SFML modules
target_link_libraries(the_quest
    sfml-graphics
    sfml-window
    sfml-system
)