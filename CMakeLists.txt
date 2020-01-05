CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

# Configure project

PROJECT( golPlayground )

# Configuration CMake Module Path

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake" )

# Choose default build type (if none specified)

IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE Debug )
ENDIF()

# Disable in-source builds

IF( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    MESSAGE( FATAL_ERROR "In-source builds are not allowed. Please create a new directory (a Build directory)." )
ENDIF()

# Compilation flags

INCLUDE( CompileFlags )

# Set prefix path if none specified

IF( NOT CMAKE_PREFIX_PATH )
    SET( CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
ENDIF()

# Configure Whimsical sources

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/Includes" )

FILE( GLOB_RECURSE GOL_SOURCES "Sources/*.cpp" )
FILE( GLOB_RECURSE GOL_HEADERS "Includes/*.hpp" )
FILE( GLOB_RECURSE GOL_INLINES "Includes/*.inl" )

FILE(
    GLOB_RECURSE
    GOL_FILES
    ${GOL_SOURCES}
    ${GOL_HEADERS}
    ${GOL_INLINES}
)

# Configuring SFML

FIND_PACKAGE( SFML 2.5.1 REQUIRED system window graphics network audio )

if(NOT SFML_FOUND)
    message(FATAL_ERROR "Could not find SFML")
endif()

INCLUDE_DIRECTORIES( ${SFML_INCLUDE_DIR} )

# Configuring TGUI

FIND_PACKAGE( TGUI 0.8 REQUIRED )

if(NOT TGUI_FOUND)
    message(FATAL_ERROR "Could not find TGUI")
endif()

INCLUDE_DIRECTORIES( ${TGUI_INCLUDE_DIR} )

# Configuring Catch2

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/Submodules/Catch2/single_include" )

# Configuring Doxygen target

FIND_PACKAGE( Doxygen )

IF( DOXYGEN_FOUND )
    SET( DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Documentation/Doxyfile.in )
    SET( DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Documentation/Doxyfile )

    CONFIGURE_FILE( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )

    ADD_CUSTOM_TARGET(
        Documentation ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Documentation
        COMMENT "Generating documentation using Doxygen"
        VERBATIM
    )
ELSE()
    MESSAGE( STATUS "${PROJECT_NAME} : Doxygen need to be installed to generate documentation" )
ENDIF()

# Configuring targets

ADD_SUBDIRECTORY( Application )
ADD_SUBDIRECTORY( Tests )