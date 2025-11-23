cmake_minimum_required(VERSION 3.14)
set(TARGET_NAME base_MVP)

message(" ")
message("+---------------------------------------------------------------------+")
message("| ${TARGET_NAME}                                                      |")
message("+---------------------------------------------------------------------+")
message(" ")

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

set(HDR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(HEADERS_DIR ${HDR_DIR}/mvp)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS 
    ${HEADERS_DIR}/model/IModel.hpp

    ${HEADERS_DIR}/presenter/IPresenter.hpp
    ${HEADERS_DIR}/presenter/Presenter.hpp

    ${HEADERS_DIR}/utility/Signal.hpp

   ${HEADERS_DIR}/view/IViewItem.hpp
)

add_library(${TARGET_NAME} INTERFACE ${HEADERS})

target_include_directories(${TARGET_NAME} INTERFACE ${HDR_DIR})

# Qt5 or Qt6 — unified API
target_link_libraries(${TARGET_NAME}
    INTERFACE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
