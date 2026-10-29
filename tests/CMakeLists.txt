cmake_minimum_required(VERSION 3.20)
project(EzChordTests C)
set(CMAKE_C_STANDARD 11)

# Host-only tests: pure modules, no Pico SDK. Build separately from firmware:
#   cmake -S tests -B build-tests && cmake --build build-tests && ctest --test-dir build-tests
enable_testing()
set(SRC ${CMAKE_CURRENT_LIST_DIR}/../EzChord)
add_executable(ezchord_tests test_main.c
    ${SRC}/chord_voicing.c ${SRC}/chord_player.c ${SRC}/display_layout.c ${SRC}/debounce.c ${SRC}/key_matrix.c ${SRC}/performance.c ${SRC}/quality_mask.c)
target_include_directories(ezchord_tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
add_test(NAME ezchord_tests COMMAND ezchord_tests)
