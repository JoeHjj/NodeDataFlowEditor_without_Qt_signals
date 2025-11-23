cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME node_editor_core)

message(" ")
message("+---------------------------------------------------------------------+")
message("| ${TARGET_NAME}                                                      |")
message("+---------------------------------------------------------------------+")
message(" ")

# -----------------------------------------------------------
# Options
# -----------------------------------------------------------
option(ENABLE_TESTS "Enable unit tests" OFF)

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

# -----------------------------------------------------------
# Path Sets (YOUR ORIGINAL VARIABLES)
# -----------------------------------------------------------
set(VIEW_INCLUDE_REPO view/include)
set(VIEW_HEADERS_REPO ${VIEW_INCLUDE_REPO}/core/view)
set(VIEW_SRC_REPO view/src)

set(MODEL_INCLUDE_REPO model/include)
set(MODEL_HEADERS_REPO ${MODEL_INCLUDE_REPO}/core/model)
set(MODEL_SRC_REPO model/src)

set(PRESENTER_INCLUDE_REPO presenter/include)
set(PRESENTER_HEADERS_REPO ${PRESENTER_INCLUDE_REPO}/core/presenter)
set(PRESENTER_SRC_REPO presenter/src)

# -----------------------------------------------------------
# Sources
# -----------------------------------------------------------
set(SOURCES
     ${MODEL_SRC_REPO}/EditableArrowItemModel.cpp
     ${PRESENTER_SRC_REPO}/EditableArrowItemPresenter.cpp
     ${VIEW_SRC_REPO}/EditableArrowItemView.cpp

     ${MODEL_SRC_REPO}/PortItemModel.cpp
     ${PRESENTER_SRC_REPO}/PortItemPresenter.cpp
     ${VIEW_SRC_REPO}/PortItemView.cpp

     ${MODEL_SRC_REPO}/ConnectionPathModel.cpp
     ${PRESENTER_SRC_REPO}/ConnectionPathPresenter.cpp
     ${VIEW_SRC_REPO}/ConnectionPathView.cpp

     ${MODEL_SRC_REPO}/NodeItemModel.cpp
     ${PRESENTER_SRC_REPO}/NodeItemPresenter.cpp
     ${VIEW_SRC_REPO}/NodeItemView.cpp

     ${VIEW_SRC_REPO}/GraphScene.cpp
     ${VIEW_SRC_REPO}/GraphView.cpp
)

# -----------------------------------------------------------
# Headers
# -----------------------------------------------------------
set(HEADERS
    ${MODEL_HEADERS_REPO}/EditableArrowItemModel.hpp
    ${PRESENTER_HEADERS_REPO}/EditableArrowItemPresenter.hpp
    ${VIEW_HEADERS_REPO}/EditableArrowItemView.hpp
    
    ${MODEL_HEADERS_REPO}/PortItemModel.hpp
    ${PRESENTER_HEADERS_REPO}/PortItemPresenter.hpp
    ${VIEW_HEADERS_REPO}/PortItemView.hpp

    ${MODEL_HEADERS_REPO}/NodeItemModel.hpp
    ${PRESENTER_HEADERS_REPO}/NodeItemPresenter.hpp
    ${VIEW_HEADERS_REPO}/NodeItemView.hpp

    ${MODEL_HEADERS_REPO}/ConnectionPathModel.hpp
    ${PRESENTER_HEADERS_REPO}/ConnectionPathPresenter.hpp
    ${VIEW_HEADERS_REPO}/ConnectionPathView.hpp

    ${VIEW_HEADERS_REPO}/GraphScene.hpp
    ${VIEW_HEADERS_REPO}/GraphView.hpp

)

# -----------------------------------------------------------
# Library target
# -----------------------------------------------------------
add_library(${TARGET_NAME}
${HEADERS}
${SOURCES}
)

target_sources(${TARGET_NAME}
    PRIVATE ${SOURCES}
    PUBLIC ${HEADERS}
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        ${VIEW_INCLUDE_REPO}
        ${UTILITY_INCLUDE_REPO}
        ${TAGGABLE_INCLUDE_REPO}
        ${MODEL_INCLUDE_REPO}
        ${PRESENTER_INCLUDE_REPO}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Qt5 or Qt6 — unified API
target_link_libraries(${TARGET_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        base_MVP
        node_editor_common
)

# -----------------------------------------------------------
# Tests (optional)
# -----------------------------------------------------------
if (ENABLE_TESTS)
    add_subdirectory(tests)
endif()
