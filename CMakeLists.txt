cmake_minimum_required(VERSION 3.16)

project(Roboticus_control_center VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Packages
find_package(Qt6 REQUIRED COMPONENTS Quick Graphs SerialPort Gui)

qt_standard_project_setup(REQUIRES 6.11)

# 2. Path Definitions
set(APP_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(APP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(APP_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui")
set(APP_QML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qml")
set(APP_PARSER_SRC_DIR "${APP_SRC_DIR}/parser")

# 3. Add External Libraries
add_subdirectory(libs/qmsgpack)

# 4. Define the Main Executable
qt_add_executable(appRoboticus_control_center
    "${APP_SRC_DIR}/main.cpp"
)

# 5. Define the QML Module
qt_add_qml_module(appRoboticus_control_center
    URI com.Roboticus.ControlCenter
    VERSION 1.0

    # Use relative paths here
    QML_FILES
        "ui/Main.qml"
        "qml/SensorPanel.qml"
        "qml/SensorInfo.qml"
        "qml/AddItem.qml"
        "qml/SinglePointSeries.qml"
        "qml/VectorArrow.qml"
        "qml/VectorInfo.qml"
        "qml/MonitoringPanel.qml"
        "qml/Error.qml"
        "qml/TitleBar.qml"
        "qml/Timeline.qml"
        "qml/AppCoordinator.qml"

    RESOURCES
        "qml/assets/SVG/add.svg"
        "qml/assets/SVG/remove.svg"
        "qml/assets/SVG/arrow.svg"
        "qml/assets/SVG/trash.svg"
        "qml/assets/SVG/arrow_left_green.svg"
        "qml/assets/SVG/arrow_left_grey.svg"
        "qml/assets/SVG/arrow_right_green.svg"
        "qml/assets/SVG/arrow_right_grey.svg"
        "qml/assets/SVG/download.svg"
        "qml/assets/SVG/upload.svg"

    SOURCES

        "include/controllers/AppController.h"
        "src/AppController.cpp"
        "include/SensorController.h"
        "src/SensorController.cpp"
        "include/VectorController.h"
        "src/VectorController.cpp"
        "include/SensorModel.h"
        "src/SensorModel.cpp"
        "include/VectorModel.h"
        "src/VectorModel.cpp"
        "include/SerialParser.h"
        "src/SerialParser.cpp"
        "include/SerialPortManager.h"
        "src/SerialPortManager.cpp"
        "include/parser/SerialFrameExtractor.h"
        "src/parser/SerialFrameExtractor.cpp"
        "src/parser/SnapshotStore.cpp"
        "src/parser/SnapshotLoader.cpp"
)

# 6. Include and Link
target_include_directories(appRoboticus_control_center PRIVATE
    "${APP_INCLUDE_DIR}"
    "${APP_INCLUDE_DIR}/controllers"
    "libs"
)

target_link_libraries(appRoboticus_control_center
    PRIVATE
        Qt6::Quick
        Qt6::Graphs
        Qt6::SerialPort
        Qt6::Gui
        qmsgpack
)

# 7. OS Specific Properties
set_target_properties(appRoboticus_control_center PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 8. Installation (Optional but good practice)
include(GNUInstallDirs)
install(TARGETS appRoboticus_control_center
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)