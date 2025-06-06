cmake_minimum_required(VERSION 3.8)
project(wpa_ctrl C CXX)
set(EXECUTABLE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_definitions(
    -DCONFIG_AP=y
    -DCONFIG_AUTOSCAN_EXPONENTIAL=y
    -DCONFIG_AUTOSCAN_PERIODIC=y
    -DCONFIG_BACKEND=file
    -DCONFIG_BGSCAN_SIMPLE=y
    -DCONFIG_CTRL_IFACE=y
    -DCONFIG_CTRL_IFACE_UNIX=y
    -DCONFIG_DEBUG_FILE=y
    -DCONFIG_DRIVER_NL80211=y
    -DCONFIG_DRIVER_WEXT=y
    -DCONFIG_DRIVER_WIRED=y
    -DCONFIG_EAP_FAST=y
    -DCONFIG_EAP_GTC=y
    -DCONFIG_EAP_LEAP=y
    -DCONFIG_EAP_MD5=y
    -DCONFIG_EAP_MSCHAPV2=y
    -DCONFIG_EAP_OTP=y
    -DCONFIG_EAP_PEAP=y
    -DCONFIG_EAP_PWD=y
    -DCONFIG_EAP_TLS=y
    -DCONFIG_EAP_TTLS=y
    -DCONFIG_HS20=y
    -DCONFIG_HT_OVERRIDES=y
    -DCONFIG_IBSS_RSN=y
    -DCONFIG_IEEE80211AC=y
    -DCONFIG_IEEE80211N=y
    -DCONFIG_IEEE80211R=y
    -DCONFIG_IEEE80211W=y
    -DCONFIG_IEEE8021X_EAPOL=y
    -DCONFIG_INTERWORKING=y
    -DCONFIG_IPV6=y
    -DCONFIG_LIBNL32=y
    -DCONFIG_NO_RANDOM_POOL=y
    -DCONFIG_P2P=y
    -DCONFIG_PEERKEY=y
    -DCONFIG_PKCS12=y
    -DCONFIG_READLINE=y
    -DCONFIG_SMARTCARD=y
    -DCONFIG_TDLS=y
    -DCONFIG_VHT_OVERRIDES=y
    -DCONFIG_WIFI_DISPLAY=y
    -DCONFIG_WPS=y
    -DCONFIG_WPS_NFC=y
    -DCONFIG_TLS_DEFAULT_CIPHERS="DEFAULT:!EXP:!LOW:3DES")
if(ANDROID)
    add_definitions(
        -DAID_SYSTEM=1000  #/* system server */
        -DAID_WIFI=1010  #/* wifi subsystem */
        -DANDROID_SOCKET_NAMESPACE_RESERVED=1
        )
endif()
add_library(wpa_ctrl wpa_ctrl.c utils/os_unix.c utils/wpabuf.c)
include_directories(wpa_ctrl PUBLIC 
   "utils"   "./"
   ${CDROID_INCLUDE_DIRS}
   ${CDROID_DEPINCLUDES}
   ${CMAKE_BINARY_DIR}/include
   ${CMAKE_BINARY_DIR}/include/gui
   ${CMAKE_BINARY_DIR}/include/porting)
target_compile_options(wpa_ctrl PUBLIC -Wall -Wextra -Wno-variadic-macros)

add_executable(wpatest main.cc wpasockclient.cc wifiscanner.cc)
target_link_libraries(wpatest wpa_ctrl cdroid)
