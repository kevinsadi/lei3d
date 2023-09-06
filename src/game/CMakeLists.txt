cmake_minimum_required(VERSION 3.14)
project(lei3d)

file(GLOB_RECURSE GAME_SOURCE
        "*.cpp"
        )

file(GLOB_RECURSE GAME_HEADER
        "*.hpp"
        )

set(SKYLEI_SOURCE ${GAME_HEADER} ${GAME_SOURCE} PARENT_SCOPE)
