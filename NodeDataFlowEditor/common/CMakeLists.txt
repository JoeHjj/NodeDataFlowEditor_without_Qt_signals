cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME node_editor_common)

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
set(VIEW_HEADERS_REPO ${VIEW_INCLUDE_REPO}/common/view)
set(VIEW_SRC_REPO view/src)

set(UTILITY_INCLUDE_REPO utility/include)
set(UTILITY_HEADERS_REPO ${UTILITY_INCLUDE_REPO}/common/utility)
set(UTILITY_SRC_REPO utility/src)

set(TAGGABLE_INCLUDE_REPO taggable/include)
set(TAGGABLE_HEADERS_REPO ${TAGGABLE_INCLUDE_REPO}/common/taggable)
set(MODEL_INCLUDE_REPO model/include)
set(MODEL_HEADERS_REPO ${MODEL_INCLUDE_REPO}/common/model)
set(MODEL_SRC_REPO model/src)

set(PRESENTER_INCLUDE_REPO presenter/include)
set(PRESENTER_HEADERS_REPO ${PRESENTER_INCLUDE_REPO}/common/presenter)
set(PRESENTER_SRC_REPO presenter/src)

# -----------------------------------------------------------
# Sources
# -----------------------------------------------------------
set(SOURCES
    ${VIEW_SRC_REPO}/AbstractItemView.cpp
    ${MODEL_SRC_REPO}/AbstractItemModel.cpp
    ${PRESENTER_SRC_REPO}/AbstractItemPresenter.cpp

    ${VIEW_SRC_REPO}/AbstractPathView.cpp
    ${MODEL_SRC_REPO}/AbstractPathModel.cpp
    ${PRESENTER_SRC_REPO}/AbstractPathPresenter.cpp
)

# -----------------------------------------------------------
# Headers
# -----------------------------------------------------------
set(HEADERS
    ${MODEL_HEADERS_REPO}/AbstractItemModel.hpp
    ${PRESENTER_HEADERS_REPO}/AbstractItemPresenter.hpp
    ${VIEW_HEADERS_REPO}/AbstractItemView.hpp

    ${MODEL_HEADERS_REPO}/AbstractPathModel.hpp
    ${PRESENTER_HEADERS_REPO}/AbstractPathPresenter.hpp
    ${VIEW_HEADERS_REPO}/AbstractPathView.hpp

    ${TAGGABLE_HEADERS_REPO}/Taggable.hpp
    ${TAGGABLE_HEADERS_REPO}/TagApplicator.hpp
    ${TAGGABLE_HEADERS_REPO}/TagRegistry.hpp

    ${UTILITY_HEADERS_REPO}/ConnectionPortData.hpp
    ${UTILITY_HEADERS_REPO}/ConnectionInfo.hpp
    ${UTILITY_HEADERS_REPO}/GraphicsProperties.hpp
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
)

# -----------------------------------------------------------
# Tests (optional)
# -----------------------------------------------------------
if (ENABLE_TESTS)
    add_subdirectory(tests)
endif()
