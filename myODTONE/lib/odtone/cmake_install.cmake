# Install script for directory: /home/phra/tesi/802_21/myODTONE/lib/odtone

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/odtone" TYPE FILE FILES
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/base.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/bind_rv.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/buffer.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/cast.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/conf.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/debug.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/exception.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/list_node.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/logger.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/namespace.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/random.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/string.hpp"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/strutil.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/odtone" TYPE DIRECTORY FILES "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/mih")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/odtone" TYPE DIRECTORY FILES "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/sap")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/odtone" TYPE DIRECTORY FILES "/home/phra/tesi/802_21/myODTONE/lib/odtone/../../inc/odtone/net")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libodtone.so.0.5.0"
      "$ENV{DESTDIR}/usr/local/lib/libodtone.so.0.5"
      "$ENV{DESTDIR}/usr/local/lib/libodtone.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libodtone.so.0.5.0;/usr/local/lib/libodtone.so.0.5;/usr/local/lib/libodtone.so")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/libodtone.so.0.5.0"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/libodtone.so.0.5"
    "/home/phra/tesi/802_21/myODTONE/lib/odtone/libodtone.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libodtone.so.0.5.0"
      "$ENV{DESTDIR}/usr/local/lib/libodtone.so.0.5"
      "$ENV{DESTDIR}/usr/local/lib/libodtone.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

