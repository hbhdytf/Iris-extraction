# Install script for directory: /home/sandy/Iris/OPENCV/doc

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
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
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

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencv/doc" TYPE FILE FILES
    "/home/sandy/Iris/OPENCV/doc/ChangeLog.htm"
    "/home/sandy/Iris/OPENCV/doc/haartraining.htm"
    "/home/sandy/Iris/OPENCV/doc/index.htm"
    "/home/sandy/Iris/OPENCV/doc/packaging.txt"
    "/home/sandy/Iris/OPENCV/doc/license.txt"
    "/home/sandy/Iris/OPENCV/doc/README.txt"
    "/home/sandy/Iris/OPENCV/doc/CMakeLists.txt"
    "/home/sandy/Iris/OPENCV/doc/opencv.jpg"
    "/home/sandy/Iris/OPENCV/doc/opencv-logo.png"
    "/home/sandy/Iris/OPENCV/doc/opencv-logo2.png"
    "/home/sandy/Iris/OPENCV/doc/opencv.pdf"
    "/home/sandy/Iris/OPENCV/doc/pattern.pdf"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencv/doc/papers" TYPE FILE FILES
    "/home/sandy/Iris/OPENCV/doc/papers/algo_tracking.pdf"
    "/home/sandy/Iris/OPENCV/doc/papers/camshift.pdf"
    "/home/sandy/Iris/OPENCV/doc/papers/avbpa99.ps"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencv/doc/vidsurv" TYPE FILE FILES
    "/home/sandy/Iris/OPENCV/doc/vidsurv/Blob_Tracking_Modules.doc"
    "/home/sandy/Iris/OPENCV/doc/vidsurv/Blob_Tracking_Tests.doc"
    "/home/sandy/Iris/OPENCV/doc/vidsurv/TestSeq.doc"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")

