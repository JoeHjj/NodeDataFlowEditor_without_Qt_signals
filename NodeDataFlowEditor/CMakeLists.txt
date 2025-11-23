cmake_minimum_required(VERSION 3.16)
project(NodeDataFlowEditor LANGUAGES CXX)

set(TARGET_NAME NodeDataFlowEditor)

message("")
message("+---------------------------------------------------------------------+")
message("| ${TARGET_NAME}                                                      |")
message("+---------------------------------------------------------------------+")
message("")

option(ENABLE_TESTS "Enable unit tests" OFF)

# -----------------------------
# Qt: Prefer Qt6, fallback Qt5
# -----------------------------
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if (Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_PACKAGE Qt6)
else()
    message(STATUS "Qt6 not found, using Qt5")
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
    set(QT_PACKAGE Qt5)
endif()

# Make these global if subdirs need them during configure
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(common)
add_subdirectory(core)

# --------------------------------
# Umbrella INTERFACE "meta" target
# --------------------------------
add_library(${TARGET_NAME} INTERFACE)

# Expose Qt + sub-libraries to consumers of NodeDataFlowEditor
target_link_libraries(${TARGET_NAME} INTERFACE
    ${QT_PACKAGE}::Core
    ${QT_PACKAGE}::Gui
    ${QT_PACKAGE}::Widgets
    base_MVP
    node_editor_common
    node_editor_core
)

# If base/core export include dirs via their own targets with PUBLIC scope,
# you don't need to repeat them here. Otherwise you could:
# target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/base/include ${CMAKE_CURRENT_SOURCE_DIR}/core/include)

if (ENABLE_TESTS)
    add_subdirectory(tests)
endif()
