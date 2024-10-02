cmake_minimum_required(VERSION 3.5)

project(CPP4_3D_Viewer_V2_0-1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets OpenGL Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets OpenGL Gui REQUIRED)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGLWidgets REQUIRED)
endif()

set(PROJECT_SOURCES
        view/main.cc
        view/resourses.qrc
        view/mainwindow.h
        view/mainwindow.cc
        view/mainwindow.ui
        view/viewwidget.h
        view/viewwidget.cc
        controller/controller.h
        controller/controller.cc
        model/parser.h
        model/parser.cc
        model/preparer.h
        model/preparer.cc
        model/modeldata.h
        model/modeldata.cc
)

# for gif animation
add_subdirectory(QtGifImage-master)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CPP4_3D_Viewer_V2_0-1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(CPP4_3D_Viewer_V2_0-1 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(CPP4_3D_Viewer_V2_0-1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Qt Library
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Gui
    QtGifImage::QtGifImage
    #cereal
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()

target_link_libraries(CPP4_3D_Viewer_V2_0-1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(CPP4_3D_Viewer_V2_0-1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CPP4_3D_Viewer_V2_0-1)
endif()
