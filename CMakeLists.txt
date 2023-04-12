cmake_minimum_required(VERSION 3.5)

project(synthesizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Multimedia)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.h mainwindow.cpp
    block.h block.cpp
    graph.h graph.cpp
    signaldialog.h signaldialog.cpp
    signalvector.h signalvector.cpp
    customgraphicsscene.h customgraphicsscene.cpp
    customgraphicsview.h customgraphicsview.cpp
    signalproperties.h signalproperties.cpp
    signalprocess.h signalprocess.cpp
    assets/assets.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(synthesizer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET graphics01 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(synthesizer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(synthesizer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(synthesizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(synthesizer PRIVATE Qt${QT_VERSION_MAJOR}::Charts)
target_link_libraries(synthesizer PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)

set_target_properties(synthesizer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS synthesizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(synthesizer)
endif()
