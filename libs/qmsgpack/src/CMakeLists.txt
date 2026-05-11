set(qmsgpack_srcs 
    msgpack.cpp 
    msgpackcommon.cpp 
    msgpackstream.cpp 
    private/pack_p.cpp 
    private/unpack_p.cpp 
    private/qt_types_p.cpp 
    stream/time.cpp 
    stream/geometry.cpp
)
set(qmsgpack_headers 
    msgpack.h 
    msgpackstream.h 
    msgpackcommon.h 
    msgpack_export.h 
    endianhelper.h
)
set(qmsgpack_stream_headers 
    stream/location.h 
    stream/time.h 
    stream/geometry.h
)

# Default to STATIC if not defined, usually better for submodules
if(NOT DEFINED COMPILE_TYPE)
  set(COMPILE_TYPE STATIC) 
endif()

add_library(qmsgpack ${COMPILE_TYPE} ${qmsgpack_srcs} ${qmsgpack_headers})

target_compile_definitions(qmsgpack PUBLIC MSGPACK_STATIC)

# Link against the libraries we found in the parent CMake (Qt6 or Qt5)
target_link_libraries(qmsgpack PUBLIC ${QT_LIBRARIES})

# This ensures that when you #include <qmsgpack/msgpack.h>, 
# CMake knows where to look.
target_include_directories(qmsgpack PUBLIC 
    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/msgpackcommon.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/msgpackcommon.h"
  @ONLY)

if (NOT ANDROID)
    set_target_properties(qmsgpack PROPERTIES
        VERSION ${QMSGPACK_MAJOR}.${QMSGPACK_MINOR}.${QMSGPACK_VERSION}
        SOVERSION ${QMSGPACK_MAJOR})
endif()

set_target_properties(qmsgpack PROPERTIES
    DEFINE_SYMBOL MSGPACK_MAKE_LIB
    PUBLIC_HEADER "${qmsgpack_headers}"
)

# Standard installation paths
install(TARGETS qmsgpack EXPORT qmsgpack-export
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}/qmsgpack
)

install(FILES ${qmsgpack_stream_headers} DESTINATION ${INCLUDE_INSTALL_DIR}/qmsgpack/stream)

# Generate thin wrapper headers in the build tree so users can include
# <qmsgpack/header.h> while the real headers live in `src/`.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qmsgpack)

foreach(h IN LISTS qmsgpack_headers)
    # wrapper simply forwards to the real header by including it as <header>
    set(wrapper_path "${CMAKE_CURRENT_BINARY_DIR}/qmsgpack/${h}")
    file(WRITE ${wrapper_path} "#pragma once\n#include <${h}>\n")
endforeach()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qmsgpack/stream)
foreach(h IN LISTS qmsgpack_stream_headers)
    set(wrapper_path "${CMAKE_CURRENT_BINARY_DIR}/qmsgpack/stream/${h}")
    file(WRITE ${wrapper_path} "#pragma once\n#include <stream/${h}>\n")
endforeach()