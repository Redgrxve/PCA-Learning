cmake_minimum_required(VERSION 3.16)

project(pcalearn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

if(UNIX)
    find_package(Eigen3 REQUIRED)
endif()

get_target_property(_qtcharts_location Qt${QT_VERSION_MAJOR}::Charts LOCATION)
message(STATUS "QtCharts target location: ${_qtcharts_location}")

set(PROJECT_SOURCES
        src/main.cpp
        src/gui/mainwindow.cpp
        src/gui/mainwindow.h
        src/gui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pcalearn
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/gui/chartwidget.cpp src/gui/chartwidget.h src/gui/chartwidget.ui src/gui/customchartview.cpp src/gui/customchartview.h src/gui/pcachart.cpp src/gui/pcachart.h src/gui/pcachartview.cpp src/gui/pcachartview.h
        src/models/pcadatamodel.h
        src/utils/pca.h
        src/models/pcadatamodel.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET pcalearn APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(pcalearn SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(pcalearn
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    set(EIGEN3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../../../../Libs/cpp/Eigen/")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    "src/gui"
    "src/models"
    "src/utils"
    ${EIGEN3_INCLUDE_DIR}
)

target_link_libraries(pcalearn PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.pcalearn)
endif()
set_target_properties(pcalearn PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS pcalearn
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pcalearn)
endif()
