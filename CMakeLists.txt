cmake_minimum_required(VERSION 3.14)
project(node_editor_mvp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------
# Qt: Prefer Qt6, fallback to Qt5
# -----------------------------------------------------------
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)

if (Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_PACKAGE Qt6)
else()
    message(STATUS "Qt6 not found, using Qt5")
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
    set(QT_PACKAGE Qt5)
endif()

add_subdirectory(base)
add_subdirectory(NodeDataFlowEditor)

add_executable(${PROJECT_NAME} example/main.cpp )

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
#target_include_directories(${PROJECT_NAME} PRIVATE base/include)

# Qt5 or Qt6 — unified API
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        base_MVP
        NodeDataFlowEditor
)
