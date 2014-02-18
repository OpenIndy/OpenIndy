
# Copyright (C) 2008-2013 Conrad Sanderson
# Copyright (C) 2008-2013 NICTA (www.nicta.com.au)
# Copyright (C) 2011 Clement Creusot
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.



cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(ARMA_MAJOR 3)
set(ARMA_MINOR 910)
set(ARMA_PATCH 0)

message(STATUS "Configuring Armadillo ${ARMA_MAJOR}.${ARMA_MINOR}.${ARMA_PATCH}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build_aux/cmake/Modules/")

include(CheckIncludeFileCXX)
include(CheckLibraryExists)
include(FindBoost)
include(ARMA_CheckProto)
include(ARMA_CheckMathProto)

project(armadillo CXX)

set(ARMA_USE_LAPACK  false)
set(ARMA_USE_BLAS    false)
set(ARMA_USE_ATLAS   false)
set(ARMA_USE_BOOST   false)
set(ARMA_USE_HDF5    false)
set(ARMA_USE_WRAPPER true )

# NOTE:
# Set ARMA_USE_WRAPPER to false if you're getting linking errors when compiling your programs,
# or if you prefer to directly link with BLAS and/or LAPACK.
# You will then need to link your programs with -lblas -llapack instead of -larmadillo
# If you're using OpenBLAS, link your programs with -lopenblas -llapack instead of -larmadillo


message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")


if(WIN32)
  message(STATUS "")
  message(STATUS "WARNING:")
  message(STATUS "Automatic installation is currently not available for this platform.")
  message(STATUS "Please use the manual installation, as described in the README.txt file.")
  message(STATUS "You can also use the 'include' folder directly, but you may want to edit")
  message(STATUS "'include/armadillo_bits/config.hpp' if you have LAPACK installed.")
  message(STATUS "")
endif()



#
# Find LAPACK and BLAS libraries, or their optimised versions
#

if(APPLE)
  set(ARMA_OS macos)
  
  set(ARMA_USE_LAPACK true)
  set(ARMA_USE_BLAS   true)
  
  # Under MacOS, the current version of ARMA_FindCLAPACK can get confused between
  # two incompatible versions of "clapack.h" (one provided by the system and one
  # provided by ATLAS).  As such, use of ATLAS under MacOS is disabled for now.
  
else()
  set(ARMA_OS unix)
  
  include(ARMA_FindMKL)
  include(ARMA_FindACMLMP)
  include(ARMA_FindACML)
  
  message(STATUS "   MKL_FOUND   = ${MKL_FOUND}")
  message(STATUS "ACMLMP_FOUND   = ${ACMLMP_FOUND}")
  message(STATUS "  ACML_FOUND   = ${ACML_FOUND}")
  
  if(MKL_FOUND OR ACMLMP_FOUND OR ACML_FOUND)
    
    set(ARMA_USE_BLAS   true)
    set(ARMA_USE_LAPACK true)
    
    message(STATUS "")
    message(STATUS "*** If the MKL or ACML libraries are installed in")
    message(STATUS "*** /opt or /usr/local, make sure the run-time linker can find them.")
    message(STATUS "*** On Linux systems this can be done by editing /etc/ld.so.conf")
    message(STATUS "*** or modifying the LD_LIBRARY_PATH environment variable.")
    message(STATUS "*** On systems with SELinux enabled (eg. Fedora, RHEL),")
    message(STATUS "*** you may need to change the SELinux type of all MKL/ACML libraries")
    message(STATUS "*** to fix permission problems that may occur during run-time.")
    message(STATUS "*** See README.txt for more information")
    
  else()
    
    include(ARMA_FindLAPACK)
    include(ARMA_FindOpenBLAS)
    include(ARMA_FindBLAS)
    include(ARMA_FindCLAPACK)
    include(ARMA_FindCBLAS)
    
    message(STATUS "  LAPACK_FOUND = ${LAPACK_FOUND}")
    message(STATUS "    BLAS_FOUND = ${BLAS_FOUND}")
    message(STATUS "OpenBLAS_FOUND = ${OpenBLAS_FOUND}")
    message(STATUS " CLAPACK_FOUND = ${CLAPACK_FOUND}")
    message(STATUS "   CBLAS_FOUND = ${CBLAS_FOUND}")
    
    if(LAPACK_FOUND)
      set(ARMA_USE_LAPACK true)
    endif()
    
    if(BLAS_FOUND)
      set(ARMA_USE_BLAS true)
    endif()
    
    if(OpenBLAS_FOUND AND CLAPACK_FOUND AND CBLAS_FOUND)
      message(STATUS "")
      message(STATUS "*** WARNING: both OpenBLAS and ATLAS have been found; ATLAS will not be used")
    endif()
    
    if(OpenBLAS_FOUND)
      
      set(ARMA_USE_BLAS true)
      
      message(STATUS "")
      message(STATUS "*** If the OpenBLAS library is installed in")
      message(STATUS "*** /usr/local/lib or /usr/local/lib64")
      message(STATUS "*** make sure the run-time linker can find it.")
      message(STATUS "*** On Linux systems this can be done by editing /etc/ld.so.conf")
      message(STATUS "*** or modifying the LD_LIBRARY_PATH environment variable.")
      
    else()
      
      if(CLAPACK_FOUND AND CBLAS_FOUND)
        message(STATUS "CLAPACK_INCLUDE_DIR = ${CLAPACK_INCLUDE_DIR}")
        message(STATUS "  CBLAS_INCLUDE_DIR = ${CBLAS_INCLUDE_DIR}")
        
        if(${CLAPACK_INCLUDE_DIR} STREQUAL ${CBLAS_INCLUDE_DIR})
          set(ARMA_USE_ATLAS true)
          set(ARMA_ATLAS_INCLUDE_DIR ${CLAPACK_INCLUDE_DIR})
        endif()
      endif()
      
    endif()
    
  endif()
  
endif()


if(MKL_FOUND OR ACMLMP_FOUND OR ACML_FOUND)
  
  if(MKL_FOUND)
    set(ARMA_LIBS ${ARMA_LIBS} ${MKL_LIBRARIES})
    
    if(ACMLMP_FOUND OR ACML_FOUND)
      message(STATUS "*** Intel MKL as well as AMD ACML libraries were found.")
      message(STATUS "*** Using only the MKL library to avoid linking conflicts.")
      message(STATUS "*** If you wish to use ACML instead, please link manually with")
      message(STATUS "*** acml or acml_mp instead of the armadillo run-time component.")
      message(STATUS "*** Alternatively, remove MKL from your system and rerun")
      message(STATUS "*** Armadillo's configuration using ./configure") 
    endif()
    
  else()
    
    if(ACMLMP_FOUND)
      set(ARMA_LIBS ${ARMA_LIBS} ${ACMLMP_LIBRARIES})
      
      message(STATUS "*** Both single-core and multi-core ACML libraries were found.")
      message(STATUS "*** Using only the multi-core library to avoid linking conflicts.")
    else()
      if(ACML_FOUND)
        set(ARMA_LIBS ${ARMA_LIBS} ${ACML_LIBRARIES})
      endif()
    endif()
    
  endif()
  
else()
  
  if(ARMA_USE_BLAS STREQUAL true)
    if(OpenBLAS_FOUND)
      set(ARMA_LIBS ${ARMA_LIBS} ${OpenBLAS_LIBRARIES})
    else()
      set(ARMA_LIBS ${ARMA_LIBS} ${BLAS_LIBRARIES})
    endif()
  endif()
  
  if(ARMA_USE_LAPACK STREQUAL true)
    set(ARMA_LIBS ${ARMA_LIBS} ${LAPACK_LIBRARIES})
  endif()
  
  if(ARMA_USE_ATLAS STREQUAL true)
    set(ARMA_LIBS ${ARMA_LIBS} ${CBLAS_LIBRARIES})
    set(ARMA_LIBS ${ARMA_LIBS} ${CLAPACK_LIBRARIES})
  endif()
  
endif()


if(APPLE)
  set(ARMA_LIBS ${ARMA_LIBS} "-framework Accelerate")  # or "-framework accelerate" ?
  message(STATUS "MacOS X detected. Added '-framework Accelerate' to compiler flags")
  
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(
        -stdlib=libc++
    )
  message(STATUS "Clang compiler on MacOS X detected. Added '-stdlib=libc++' to compiler flags")
  endif()

endif()


# if(CMAKE_VERSION VERSION_GREATER "2.7")
#   find_package(HDF5)
#   message(STATUS "HDF5_FOUND     = ${HDF5_FOUND}")
#   
#   if(HDF5_FOUND)
#     set(ARMA_USE_HDF5 true)
#     set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${HDF5_INCLUDE_DIRS})
#     set(ARMA_LIBS ${ARMA_LIBS} ${HDF5_LIBRARIES})
#   endif()
# endif()


message(STATUS "")
message(STATUS "*** The run-time library component of Armadillo will be an alias for the following libraries:")
message(STATUS "*** ARMA_LIBS = ${ARMA_LIBS}")
message(STATUS "")

find_package(Boost)

if(Boost_FOUND)
  
  message(STATUS "Boost_MAJOR_VERSION = ${Boost_MAJOR_VERSION}")
  message(STATUS "Boost_MINOR_VERSION = ${Boost_MINOR_VERSION}")
  
  if(Boost_MAJOR_VERSION GREATER 0)
    if(Boost_MINOR_VERSION GREATER 33)
      set(ARMA_USE_BOOST true)
      message(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
    endif()
  endif()
  
endif()

if(ARMA_USE_BOOST STREQUAL false)
  message(STATUS "Boost libraries either not found or their version is too low (1.34 or later is good).")
  message(STATUS "( It's possible that CMake didn't find the particular version of Boost you may have. )")
  message(STATUS "( If that's the case, please edit include/armadillo_bits/config.hpp manually. )")
endif()


# If Boost libraries were found, explicitly check if Boost's date_time library is also present.

if(ARMA_USE_BOOST STREQUAL true)
  set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${Boost_INCLUDE_DIR})
  CHECK_INCLUDE_FILE_CXX("boost/date_time/posix_time/posix_time.hpp" ARMA_USE_BOOST_DATE)
endif()

ARMA_CHECK_MATH_PROTO("isfinite" "std" "cmath"  ARMA_HAVE_STD_ISFINITE)
ARMA_CHECK_MATH_PROTO("isinf"    "std" "cmath"  ARMA_HAVE_STD_ISINF)
ARMA_CHECK_MATH_PROTO("isnan"    "std" "cmath"  ARMA_HAVE_STD_ISNAN)
ARMA_CHECK_MATH_PROTO("log1p"    ""    "cmath"  ARMA_HAVE_LOG1P)

ARMA_CHECK_PROTO("snprintf"     "std" "cstdio"     ARMA_HAVE_STD_SNPRINTF)
ARMA_CHECK_PROTO("gettimeofday" ""    "sys/time.h" ARMA_HAVE_GETTIMEOFDAY)


message(STATUS "Generating 'include/armadillo_bits/config.hpp'")
configure_file(include/armadillo_bits/config.hpp.cmake include/armadillo_bits/config.hpp)

message(STATUS "Generating 'examples/Makefile'")
configure_file(examples/Makefile.cmake examples/Makefile)


if(ARMA_USE_BOOST STREQUAL true)
  include_directories(include ${Boost_INCLUDE_DIR})
else()
  include_directories(include)
endif()


## For any library that is not in a default location,
## embed its path into the Armadillo runtime library.
## Examples of default locations are "/lib", "/usr/lib",
## or as specified in "/etc/ld.so.conf".
##
## Path embedding is not recommended unless you know
## what you're doing.  It might be better to add the
## path to the "/etc/ld.so.conf" file and then run "ldconfig".
##
#set(CMAKE_SKIP_BUILD_RPATH  FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH  FALSE)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)


# work around a silly limitation in Mac OS X
if(APPLE)
  if(${ARMA_MINOR} GREATER 99)
    math(EXPR ARMA_MINOR_ALT "${ARMA_MINOR} / 10")
  else()
    set(ARMA_MINOR_ALT ${ARMA_MINOR})
  endif()
else()
  set(ARMA_MINOR_ALT ${ARMA_MINOR})
endif()


# necessary when linking with Intel MKL on Linux systems
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed")
  message(STATUS "CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
endif()

add_library( armadillo SHARED src/wrap_libs )
target_link_libraries( armadillo ${ARMA_LIBS} )
set_target_properties(armadillo PROPERTIES VERSION ${ARMA_MAJOR}.${ARMA_MINOR_ALT}.${ARMA_PATCH} SOVERSION ${ARMA_MAJOR})


################################################################################
# INSTALL CONFIGURATION


# As Red Hat Enterprise Linux (and related systems such as Fedora)
# does not search /usr/local/lib by default, we need to place the
# library in either /usr/lib or /usr/lib64

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  message(STATUS "*** CMAKE_INSTALL_PREFIX was initalised by cmake to the default value of ${CMAKE_INSTALL_PREFIX}")
  message(STATUS "*** CMAKE_INSTALL_PREFIX changed to /usr")
  unset(CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
  #set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "install prefix" FORCE)
endif()

# Allow for the "lib" directory to be specified on the command line
if(NOT INSTALL_LIB_DIR)
  set(INSTALL_LIB_DIR "lib")
  if(UNIX AND NOT APPLE)   # I don't know how Mac OS handles 64 bit systems
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      message(STATUS "*** Detected 64 bit system")
      # use "lib64" only on systems that have it (eg. Fedora, Red Hat).
      # if installing in /usr/local/, the use of lib64 might be unreliable on systems which have /usr/local/lib64 but don't use it
      if(IS_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib64")
        unset(INSTALL_LIB_DIR)
        set(INSTALL_LIB_DIR "lib64")
        message(STATUS "*** ${CMAKE_INSTALL_PREFIX}/lib64/ exists, so destination directory for the run-time library changed to ${CMAKE_INSTALL_PREFIX}/lib64/")
        message(STATUS "*** Your system and/or compiler must search ${CMAKE_INSTALL_PREFIX}/lib64/ during linking")
      endif()
    endif()
  endif()
endif()

# Allow for the "include" directory to be specified on the command line

if(NOT INSTALL_INCLUDE_DIR)
  set(INSTALL_INCLUDE_DIR "include")
endif()

# We use data dir to store files shared with other programs 
# like the ArmadilloConfig.cmake file.
if(NOT INSTALL_DATA_DIR)
  set(INSTALL_DATA_DIR "share")
endif()

# Make relative paths absolute so we can write them in Config.cmake files
foreach(p LIB INCLUDE DATA)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "INSTALL_LIB_DIR      = ${INSTALL_LIB_DIR}"     )
message(STATUS "INSTALL_INCLUDE_DIR  = ${INSTALL_INCLUDE_DIR}" )
message(STATUS "INSTALL_DATA_DIR     = ${INSTALL_DATA_DIR}"    )


# Note that the trailing / character in "include/" is critical

install(DIRECTORY include/ DESTINATION ${INSTALL_INCLUDE_DIR}
PATTERN ".svn" EXCLUDE
PATTERN "*.cmake" EXCLUDE
PATTERN "*~" EXCLUDE
PATTERN "*orig" EXCLUDE
)

install(FILES ${PROJECT_BINARY_DIR}/include/armadillo_bits/config.hpp
DESTINATION ${INSTALL_INCLUDE_DIR}/armadillo_bits
)

install(TARGETS armadillo 
  DESTINATION ${INSTALL_LIB_DIR}
  EXPORT ArmadilloLibraryDepends)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
if(CMAKE_VERSION VERSION_GREATER "2.7")
 export(PACKAGE armadillo)
endif()

## LOCAL FILES
# Create ArmadilloConfig.cmake file for the use from the build tree
set(ARMADILLO_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
set(ARMADILLO_LIB_DIR      "${PROJECT_BINARY_DIR}")
set(ARMADILLO_CMAKE_DIR    "${PROJECT_BINARY_DIR}")


message(STATUS "Generating '${PROJECT_BINARY_DIR}/ArmadilloConfig.cmake'")
# copy/change config and configVersion file (modify only the @xyz@ variables)
configure_file(build_aux/cmake/InstallFiles/ArmadilloConfig.cmake.in
  "${PROJECT_BINARY_DIR}/ArmadilloConfig.cmake" @ONLY)

message(STATUS "Generating '${PROJECT_BINARY_DIR}/ArmadilloConfigVersion.cmake'")
configure_file(build_aux/cmake/InstallFiles/ArmadilloConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/ArmadilloConfigVersion.cmake" @ONLY)


# Install the export set for use with the install-tree
install(EXPORT ArmadilloLibraryDepends DESTINATION
  "${INSTALL_DATA_DIR}/Armadillo/CMake"
  COMPONENT dev)


## GLOBAL INSTALL FILES
# Create ArmadilloConfig.cmake file for the use from the install tree
# and install it
set(ARMADILLO_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")
set(ARMADILLO_LIB_DIR      "${INSTALL_LIB_DIR}")
set(ARMADILLO_CMAKE_DIR    "${INSTALL_DATA_DIR}/Armadillo/CMake")


message(STATUS "Generating '${PROJECT_BINARY_DIR}/InstallFiles/ArmadilloConfig.cmake'")
# copy/change config and configVersion file (modify only the @xyz@ variables)
configure_file(build_aux/cmake/InstallFiles/ArmadilloConfig.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/ArmadilloConfig.cmake" @ONLY)

message(STATUS "Generating '${PROJECT_BINARY_DIR}/InstallFiles/ArmadilloConfigVersion.cmake'")
configure_file(build_aux/cmake/InstallFiles/ArmadilloConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/ArmadilloConfigVersion.cmake" @ONLY)

# Install files to be found by cmake users with find_package()
install(FILES
  "${PROJECT_BINARY_DIR}/InstallFiles/ArmadilloConfig.cmake"
  "${PROJECT_BINARY_DIR}/InstallFiles/ArmadilloConfigVersion.cmake"
  DESTINATION "${ARMADILLO_CMAKE_DIR}" COMPONENT dev)
