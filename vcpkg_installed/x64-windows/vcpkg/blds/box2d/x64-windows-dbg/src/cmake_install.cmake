# Install script for directory: C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/src/64c16f51bc-7b27809cb5.clean/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/pkgs/box2d_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/src/Debug/box2d.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/bin/box2d.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d/box2dConfig.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d/box2dConfig.cmake"
         "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/src/CMakeFiles/Export/1f7e0de6fbfb5d20e365871ec89ce73e/box2dConfig.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d/box2dConfig-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d/box2dConfig.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d" TYPE FILE FILES "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/src/CMakeFiles/Export/1f7e0de6fbfb5d20e365871ec89ce73e/box2dConfig.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d" TYPE FILE FILES "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/src/CMakeFiles/Export/1f7e0de6fbfb5d20e365871ec89ce73e/box2dConfig-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/box2d" TYPE FILE FILES "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/src/box2dConfigVersion.cmake")
endif()

