project(qmsgpack)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

# Ensure AUTOMOC is on for Qt meta-object processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Build type setup
if ("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  add_definitions("-DQT_NO_DEBUG_OUTPUT")
endif ()

if (WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
endif ()

# --- Qt6 Configuration ---
# Since your main app already found Qt6, we link directly to the targets.
message(STATUS "qmsgpack: Forcing build with Qt6 targets")

set(QT_LIBRARIES Qt6::Core)
set(PC_Requires "Qt6Core")

# Optional components
option(WITH_GUI_TYPES "Build with support for QtGui types" ON)
if (WITH_GUI_TYPES)
    message(STATUS "qmsgpack: Adding Qt6::Gui support")
    list(APPEND QT_LIBRARIES Qt6::Gui)
endif()

# Installation paths
set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/${CMAKE_PROJECT_NAME}")

# Versioning
set(QMSGPACK_MAJOR  "0")
set(QMSGPACK_MINOR  "1")
set(QMSGPACK_VERSION    "0")
set(MSGPACK_QT_LIB_VERSION_STRING "${QMSGPACK_MAJOR}.${QMSGPACK_MINOR}.${QMSGPACK_VERSION}")

# Descend into source
add_subdirectory(src)

# Optional: Disable tests to speed up build
option(BUILD_TESTS "Build tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()