file(GLOB game_top_SOURCES src/*.*)

#include_directories(${PROJECT_SOURCE_DIR}/engine/src)

include_directories(src/)

add_executable(BCGame ${game_top_SOURCES})

source_group("BcGame\\" FILES ${game_top_SOURCES})

target_link_libraries(BCGame BearClaw)
