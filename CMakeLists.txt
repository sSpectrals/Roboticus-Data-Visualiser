cmake_minimum_required(VERSION 3.16)

project(Roboticus_control_center VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Packages
find_package(Qt6 REQUIRED COMPONENTS Quick Graphs SerialPort Gui)

qt_standard_project_setup(REQUIRES 6.11)

# 2. Add External Libraries (qmsgpack)
add_subdirectory(libs/qmsgpack)

# 3. Define the Main Executable
qt_add_executable(appRoboticus_control_center
    "src/main.cpp"
)

# 4. Define the QML Module
# This combines the logic and UI into one importable URI.
qt_add_qml_module(appRoboticus_control_center
    URI com.Roboticus.ControlCenter
    VERSION 1.0

    # Virtual paths in QRC will match these relative paths
    QML_FILES
        "ui/Main.qml"
        "ui/components/SensorPanel.qml"
        "ui/components/SensorInfo.qml"

        "ui/components/SinglePointSeries.qml"
        "ui/components/VectorArrow.qml"
        "ui/components/VectorInfo.qml"
        "ui/components/MonitoringPanel.qml"
        "ui/components/Error.qml"
        "ui/components/ConnectionBar.qml"
        "ui/components/Timeline.qml"


    RESOURCES
        "assets/SVG/add.svg"
        "assets/SVG/remove.svg"
        "assets/SVG/arrow.svg"
        "assets/SVG/trash.svg"
        "assets/SVG/arrow_left_green.svg"
        "assets/SVG/arrow_left_grey.svg"
        "assets/SVG/arrow_right_green.svg"
        "assets/SVG/arrow_right_grey.svg"
        "assets/SVG/download.svg"
        "assets/SVG/upload.svg"

    SOURCES
        # Controllers (Contain QML_ELEMENT)
        "include/controllers/AppController.h"
        "src/controllers/AppController.cpp"
        "include/controllers/SensorController.h"
        "src/controllers/SensorController.cpp"
        "include/controllers/VectorController.h"
        "src/controllers/VectorController.cpp"
        "include/controllers/ControllerUtils.h"

        # Models
        "include/models/SensorModel.h"
        "src/models/SensorModel.cpp"
        "include/models/VectorModel.h"
        "src/models/VectorModel.cpp"

        # IO (Hardware/Stream Handling)
        "include/io/SerialParser.h"
        "src/io/SerialParser.cpp"
        "include/io/SerialPortManager.h"
        "src/io/SerialPortManager.cpp"

        # Parser (Pure Logic)
        "include/parser/FrameTypes.h"
        "include/parser/SerialFrameExtractor.h"
        "src/parser/SerialFrameExtractor.cpp"
        "include/parser/SnapshotStore.h"
        "src/parser/SnapshotStore.cpp"
        "include/parser/SnapshotLoader.h"
        "src/parser/SnapshotLoader.cpp"
        QML_FILES ui/components/StyledComboBox.qml
)

# 5. Include Directories
# This allows you to use #include "io/SerialParser.h" instead of relative paths
target_include_directories(appRoboticus_control_center PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/controllers"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/models"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/io"
   "${CMAKE_CURRENT_SOURCE_DIR}/include/parser"
    "libs" # For qmsgpack
)

# 6. Link Libraries
target_link_libraries(appRoboticus_control_center
    PRIVATE
        Qt6::Quick
        Qt6::Graphs
        Qt6::SerialPort
        Qt6::Gui
        qmsgpack
)

# 7. OS Specific Configuration
set_target_properties(appRoboticus_control_center PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 8. Installation
include(GNUInstallDirs)
install(TARGETS appRoboticus_control_center
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)