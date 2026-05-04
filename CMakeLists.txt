cmake_minimum_required(VERSION 3.16)

project(Roboticus_control_center VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Graphs SerialPort Gui)

qt_standard_project_setup(REQUIRES 6.11)

set(APP_SRC_DIR "src")
set(APP_INCLUDE_DIR "include")
set(APP_UI_DIR "ui")
set(APP_QML_DIR "qml")
set(APP_PARSER_INCLUDE_DIR "${APP_INCLUDE_DIR}/parser")
set(APP_PARSER_SRC_DIR "${APP_SRC_DIR}/parser")

qt_add_executable(appRoboticus_control_center
    "${APP_SRC_DIR}/main.cpp"
)

add_subdirectory(libs/qmsgpack)

add_library(domain_models STATIC
    "${APP_INCLUDE_DIR}/SensorModel.h"
    "${APP_SRC_DIR}/SensorModel.cpp"
    "${APP_INCLUDE_DIR}/VectorModel.h"
    "${APP_SRC_DIR}/VectorModel.cpp"
)
target_include_directories(domain_models PUBLIC "${APP_INCLUDE_DIR}")
target_link_libraries(domain_models PUBLIC Qt6::Core Qt6::Qml Qt6::Gui)

add_library(app_controllers STATIC
    "${APP_INCLUDE_DIR}/SensorController.h"
    "${APP_SRC_DIR}/SensorController.cpp"
    "${APP_INCLUDE_DIR}/VectorController.h"
    "${APP_SRC_DIR}/VectorController.cpp"
    "${APP_INCLUDE_DIR}/controllers/ControllerUtils.h"
)
target_include_directories(app_controllers PUBLIC "${APP_INCLUDE_DIR}")
target_link_libraries(app_controllers PUBLIC domain_models Qt6::Core Qt6::Qml Qt6::Gui)

add_library(io_serial STATIC
    "${APP_INCLUDE_DIR}/SerialParser.h"
    "${APP_SRC_DIR}/SerialParser.cpp"
    "${APP_INCLUDE_DIR}/SerialPortManager.h"
    "${APP_SRC_DIR}/SerialPortManager.cpp"
    "${APP_PARSER_INCLUDE_DIR}/FrameTypes.h"
    "${APP_PARSER_INCLUDE_DIR}/SerialFrameExtractor.h"
    "${APP_PARSER_INCLUDE_DIR}/FrameDecoder.h"
    "${APP_PARSER_INCLUDE_DIR}/SnapshotStore.h"
    "${APP_PARSER_INCLUDE_DIR}/SnapshotLoader.h"
    "${APP_PARSER_SRC_DIR}/SerialFrameExtractor.cpp"
    "${APP_PARSER_SRC_DIR}/FrameDecoder.cpp"
    "${APP_PARSER_SRC_DIR}/SnapshotStore.cpp"
    "${APP_PARSER_SRC_DIR}/SnapshotLoader.cpp"
)
target_include_directories(io_serial PUBLIC "${APP_INCLUDE_DIR}")
target_link_libraries(io_serial PUBLIC domain_models Qt6::Core Qt6::Qml Qt6::SerialPort qmsgpack)

qt_add_qml_module(appRoboticus_control_center
    URI com.Roboticus.ControlCenter
    QML_FILES
        "${APP_UI_DIR}/Main.qml"
        "${APP_QML_DIR}/SensorPanel.qml"
        "${APP_QML_DIR}/SensorInfo.qml"
        "${APP_QML_DIR}/AddItem.qml"
        "${APP_QML_DIR}/SinglePointSeries.qml"
        "${APP_QML_DIR}/VectorArrow.qml"
        "${APP_QML_DIR}/VectorInfo.qml"
        "${APP_QML_DIR}/MonitoringPanel.qml"
        "${APP_QML_DIR}/Error.qml"
        "${APP_QML_DIR}/TitleBar.qml"
        "${APP_QML_DIR}/Timeline.qml"
        "${APP_QML_DIR}/AppCoordinator.qml"

    RESOURCES
        "${APP_QML_DIR}/assets/SVG/add.svg"
        "${APP_QML_DIR}/assets/SVG/remove.svg"
        "${APP_QML_DIR}/assets/SVG/arrow.svg"
        "${APP_QML_DIR}/assets/SVG/trash.svg"
        "${APP_QML_DIR}/assets/SVG/arrow_left_green.svg"
        "${APP_QML_DIR}/assets/SVG/arrow_left_grey.svg"
        "${APP_QML_DIR}/assets/SVG/arrow_right_green.svg"
        "${APP_QML_DIR}/assets/SVG/arrow_right_grey.svg"
        "${APP_QML_DIR}/assets/SVG/download.svg"
        "${APP_QML_DIR}/assets/SVG/upload.svg"

    SOURCES
        "${APP_INCLUDE_DIR}/SensorController.h"
        "${APP_INCLUDE_DIR}/SensorModel.h"
        "${APP_INCLUDE_DIR}/VectorController.h"
        "${APP_INCLUDE_DIR}/VectorModel.h"
        "${APP_INCLUDE_DIR}/SerialParser.h"
        "${APP_INCLUDE_DIR}/SerialPortManager.h"

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appRoboticus_control_center PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRoboticus_control_center
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appRoboticus_control_center PRIVATE
    "${APP_INCLUDE_DIR}"
    libs
)

target_link_libraries(appRoboticus_control_center
    PRIVATE Qt6::Quick Qt6::Graphs Qt6::SerialPort Qt6::Gui
            app_controllers io_serial
)

include(GNUInstallDirs)
install(TARGETS appRoboticus_control_center
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
