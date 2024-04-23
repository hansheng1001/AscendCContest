# Install script for directory: /root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/op_impl/ai_core/tbe/config/ascend310b" TYPE FILE FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/tbe/op_info_cfg/ai_core/ascend310b/aic-ascend310b-ops-info.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/op_impl/ai_core/tbe/customize_impl" TYPE DIRECTORY FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/tbe/dynamic")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/op_impl/ai_core/tbe/kernel/ascend310b" TYPE DIRECTORY OPTIONAL FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/bin/less_equal")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/op_impl/ai_core/tbe/kernel/config/ascend310b" TYPE FILE OPTIONAL FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/bin/less_equal.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/op_impl/ai_core/tbe/kernel/config/ascend310b" TYPE FILE OPTIONAL FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/bin/binary_info_config.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/../build_out/kernel/ascend310b/less_equal")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/../build_out/kernel/ascend310b" TYPE DIRECTORY OPTIONAL FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/bin/less_equal")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/../build_out/kernel/config/ascend310b/binary_info_config.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/../build_out/kernel/config/ascend310b" TYPE FILE OPTIONAL FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/bin/binary_info_config.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/../build_out/kernel/config/ascend310b/less_equal.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/../build_out/kernel/config/ascend310b" TYPE FILE OPTIONAL FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/bin/less_equal.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/framework/tensorflow" TYPE FILE FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/tbe/op_info_cfg/ai_core/npu_supported_ops.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/packages/vendors/customize/op_impl/ai_core/tbe/customize_impl/dynamic" TYPE FILE FILES "/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/op_kernel/less_equal.cpp")
endif()

