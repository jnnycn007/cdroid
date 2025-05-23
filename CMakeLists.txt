project(CDROID C CXX)
cmake_minimum_required (VERSION 3.22)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
include(cmake/depchecks.cmake)
include(cmake/common_functions.cmake)
include(cmake/options.cmake)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
   #make vcpkg happy:)
endif(COMMAND cmake_policy)

add_compile_options(-Wno-deprecated-declarations)

set(DISABLE_FORCE_DEBUG_POSTFIX ON)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
   add_definitions(-DDEBUG) 
endif()
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
   add_link_options("LINKER:-rpath-link,${CMAKE_SOURCE_DIR}/src/porting/${CDROID_CHIPSET}/lib")
endif()
add_subdirectory(src/porting)
add_subdirectory(src/gui)
add_subdirectory(src/modules)
add_subdirectory(src/3rdparty/pinyin)

add_subdirectory(apps)

add_subdirectory(tests)
GetGitVersion(CDROID)
string(TIMESTAMP COMPILE_TIME "%Y%m%d_%H%M%S")
set(CPACK_PACKAGE_NAME "CDROID-${COMPILE_TIME}-${CDROID_COMMITID}")
set(CPACK_PACKAGE_VERSION ${CDROID_VERSION})
set(CPACK_GENERATOR "ZIP")  # Choose the desired generator (e.g., ZIP, TGZ, NSIS, etc.)
include(CPack)

if(BUILD_DOCS)
  include(cmake/build_doxygen.cmake)
  build_doxygen()
endif()

