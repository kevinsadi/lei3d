cmake_minimum_required(VERSION 3.14)
project(lei3d)

file(GLOB_RECURSE LEIDITOR_SOURCE
        "*.cpp"
        )

file(GLOB_RECURSE LEIDITOR_HEADER
        "*.hpp"
        )

set(EDITOR_SOURCE ${EDITOR_HEADER} ${EDITOR_SOURCE} PARENT_SCOPE)
