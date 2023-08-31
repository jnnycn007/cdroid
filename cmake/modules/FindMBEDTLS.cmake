
find_package(PkgConfig)
pkg_check_modules(PC_MBEDTLS mbedtls)

find_path(MBEDTLS_INCLUDE_DIR
    NAMES mbedtls/aes.h
    HINTS ${PC_MBEDTLS_INCLUDE_DIRS}
    ${PC_MBEDTLS_INCLUDEDIR}
)

find_library(MBCRYPT_LIBRARY
    NAMES mbedcrypto
    HINTS ${PC_MBEDTLS_LIBDIR}
    ${PC_MBEDTLS_LIBRARY_DIRS}
)
find_library(MBTLS_LIBRARY
    NAMES mbedtls 
    HINTS ${PC_MBEDTLS_LIBDIR}
    ${PC_MBEDTLS_LIBRARY_DIRS}
)
find_library(MB509_LIBRARY
    NAMES mbedx509
    HINTS ${PC_MBEDTLS_LIBDIR}
    ${PC_MBEDTLS_LIBRARY_DIRS}
)

#message(FATAL_ERROR "PC_MBEDTLS_INCLUDEDIR=${PC_MBEDTLS_INCLUDEDIR} MBEDTLS_INCLUDE_DIR=${MBEDTLS_INCLUDE_DIR} MBEDTLS_LIBRARY=${MBCRYPT_LIBRARY} ${MBTLS_LIBRARY} ${MB509_LIBRARY}")
if(MBEDTLS_INCLUDE_DIR AND MBTLS_LIBRARY)
    set(MBEDTLS_FOUND TRUE)
    set(MBEDTLS_LIBRARY ${MBCRYPT_LIBRARY} ${MBTLS_LIBRARY} ${MB509_LIBRARY})
    set(MBEDTLS_INCLUDE_DIRS ${MBEDTLS_INCLUDE_DIR})
    set(MBEDTLS_LIBRARIES ${MBEDTLS_LIBRARY})
    set(MBEDTLS_VERSION ${PC_MBEDTLS_VERSION})
    include(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(mbedtls  FOUND_VAR MBEDTLS_FOUND VERSION_VAR MBEDTLS_VERSION 
	    REQUIRED_VARS MBEDTLS_LIBRARIES MBEDTLS_LIBRARY MBEDTLS_INCLUDE_DIRS MBEDTLS_INCLUDE_DIR)
    mark_as_advanced(MBEDTLS_LIBRARIES MBEDTLS_LIBRARY MBEDTLS_INCLUDE_DIRS MBEDTLS_INCLUDE_DIR)
endif()
