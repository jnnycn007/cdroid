# -*- coding: utf-8 -*-
import os
import shutil
from datetime import datetime

mInit = True

def createDir(rootDir,dirList):
    for dir in dirList:
        folder_path = os.path.join(rootDir,dir)
        os.makedirs(folder_path)
        if mInit:
            init_file_path = os.path.join(folder_path, "init")
            open(init_file_path, "a").close()
        
def writeFile(file_path,content):
    with open(file_path, "w", encoding="utf-8") as file:
        file.write(content)
        
def touch_file(file_path):
    with open(file_path, 'a'):
        os.utime(file_path, None)
        
print("\nAuthor: Ricken - ricken.cn")
print("Update: 2024/01/19 15:35")
print("About:  A script for quickly building the Cdroid project framework\n")

project_name = ""
while True:
    # 获取用户输入的文件夹名称
    project_name = input("请输入项目名称(英文且勿使用空格以及-)：")
    
    if ' ' in project_name or '-' in project_name or not project_name:
        print("项目名称格式非法")
        continue

    # 判断文件夹是否存在
    if os.path.exists(project_name):
        # 提示用户文件夹已存在，并询问是否删除
        choice = input(f"项目 '{project_name}' 已存在!!!\nY:删除并重新创建 R:重新输入 N:退出 : ")

        if choice.lower() == "y":
            # 删除已存在的文件夹
            shutil.rmtree(project_name)
            break
        elif choice.lower() == "r":
            print("")
            continue
        else:
            print("取消项目创建。")
            exit()
    else:
        break

mInitType = input(f"是否为每个目录添加空白文件进行占位(Y:是|默认 N:否): ")
if mInitType.lower() == "n":
    mInit = False

# 创建文件夹
os.makedirs(project_name)
typeDir   = ['.vscode','3rdpart','assets','conf','docs','fonts','src']
createDir(project_name,typeDir)
assetsDir = ['anim','color','drawable','layout','mipmap','values']
createDir(os.path.join(project_name,'assets'),assetsDir)
srcDir    = ['common','data','function','net','protocol','uart','viewlibs','windows']
createDir(os.path.join(project_name,'src'),srcDir)

img = "useless"
writeFile(os.path.join(project_name,'assets','mipmap','init.png'),img)


# 写入升级文件
mUpdate = '''#!/bin/sh

if [ -d ./.git ]
then
GITLAB=`git log -1 | grep -E "^commit" | sed 's/commit //g'`
else
GITLAB=00
fi

v1=1
v2=0
v3=0
v4=0

HARD_VER_MAJ_NO=01
HARD_VER_MIN_NO=00

SOFT_VER_MAJ_NO=01
SOFT_VER_MIN_NO=00
date +%V
HARD_VER=A0.$HARD_VER_MAJ_NO.$HARD_VER_MIN_NO


DATE=$(date +"%Y-%m-%d %H:%M.%S %Z")
VER_DATE=$(date +%g).$(date +%v).$(date +%j)

VER="${v1}.${v2}.${v3}.${v4}"

ln -sf $PWD/conf/cdroid_series_conf.h  src/common/hv_series_conf.h
ln -sf $PWD/conf/defualt_config.h  src/common/hv_defualt_config.h

echo "set version $VER"
echo "set telnet version  $VER_DATE"
echo "set version TIME $DATE"
version_info=`cat conf/version.h | sed "s/RevTag/$VER/g" | sed "s/RevTIME/$DATE/g" | sed "s/HV_MAJ/$SOFT_VER_MAJ_NO/g" | sed "s/HV_MIN/$SOFT_VER_MIN_NO/g" | sed "s/RevGITLAB/$GITLAB/g" | sed "s/RevHardVer/$HARD_VER/g"`
echo "$version_info" > src/common/hv_version.h
if test -d packfs ; then
version_system=`cat packfs/hv_system_conf | sed -r "s/.*HV_TELNET_VER.+/HV_TELNET_VER=$VER_DATE/g" | sed -r "s/.*HV_VERSION_NUM.+/HV_VERSION_NUM=$VER_MAIN_NO/g" | sed -r "s/.*HV_VERSION_CODE.+/HV_VERSION_CODE=V$VER_MAJ_NO.$VER_MIN_NO/g" | sed -r "s:.*HV_OUTPUT_PATH.+:HV_OUTPUT_PATH=$WORK_APP_OUT_PATH:g"`
echo "$version_system" > packfs/hv_system_conf
fi
'''

writeFile(os.path.join(project_name,'date2ver'),mUpdate);


mCmake = '''cmake_minimum_required (VERSION 3.8)
project(__PROJECTNAME__)

execute_process(COMMAND bash date2ver  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message("CMAKE_CXX_COMPILER:${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ./
    ./3rdparty

    ./src/common
    ./src/data
    ./src/function
    ./src/net
    ./src/protocol
    ./src/uart
    ./src/viewlibs
    ./src/windows

    ${CDROID_INCLUDE_DIRS}
    ${CDROID_DEPINCLUDES}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/include/gui
    ${CMAKE_BINARY_DIR}/include/porting
)

find_package(jsoncpp CONFIG REQUIRED)
find_package(CURL REQUIRED)

add_definitions(-DCDROID_RUNNING)
if (CDROID_CHIPSET STREQUAL sigma)
    add_definitions(-DCDROID_SIGMA)
elseif(CDROID_CHIPSET STREQUAL x64)
    add_definitions(-DCDROID_X64)
endif()

aux_source_directory(./src/common PRJ_SRC_COMM)
aux_source_directory(./src/data PRJ_SRC_DATA)
aux_source_directory(./src/function PRJ_SRC_FUN)
aux_source_directory(./src/net PRJ_SRC_NET)
aux_source_directory(./src/protocol PRJ_SRC_PROTOCOL)
aux_source_directory(./src/uart PRJ_SRC_UART)
aux_source_directory(./src/viewlibs PRJ_SRC_VIEWLIB)
aux_source_directory(./src/windows PRJ_SRC_WINDOWS)
file(GLOB_RECURSE PRJ_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.cc)


# add_definitions(--include cdtypes.h)
# add_definitions(--include cdlog.h)

message("PROJECT_NAME=${PROJECT_NAME}")

add_executable(${PROJECT_NAME} ${PRJ_SRCS} ${PRJ_SRC_COMM} ${PRJ_SRC_DATA} ${PRJ_SRC_FUN} ${PRJ_SRC_NET} ${PRJ_SRC_VIEWLIB} ${PRJ_SRC_PROTOCOL} ${PRJ_SRC_UART} ${PRJ_SRC_WINDOWS})


CreatePAK(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pak ${PROJECT_SOURCE_DIR}/R.h)

target_link_libraries(${PROJECT_NAME} PRIVATE cdroid PRIVATE JsonCpp::JsonCpp ${CURL_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

'''

mCmake = mCmake.replace('__PROJECTNAME__',project_name)
writeFile(os.path.join(project_name,'CMakeLists.txt'),mCmake);

mREADME = '''## 项目名
__PROJECTNAME__

## 创建时间
__CREATETIME__

## 目录结构
~~~
- 3rdpart  第三方库
- assets   静态资源
- - anim      动画
- - color     颜色
- - drawable  样式
- - layout    布局
- - mipmap    图片
- - strings   文本
- - values    数据
- conf     配置文件(touch后编译生效,文件生成于src/common)
- docs     项目文档
- fonts    项目字体
- src      功能代码
- - common    常用函数
- - data      数据存储
- - function  功能函数
- - net       网络相关
- - protocol  通信协议
- - uart      串口相关
- - viewlibs  自定义页面
- - windows   页面函数
~~~

## 声明
**Power By [Cdroid](https://gitee.com/houstudio/cdroid)**

Create By Ricken's automation script'''

current_time = datetime.now()
formatted_time = current_time.strftime("%Y-%m-%d %H:%M:%S")
mREADME = mREADME.replace('__PROJECTNAME__',project_name).replace('__CREATETIME__',formatted_time)
writeFile(os.path.join(project_name,'README.md'),mREADME);

mMainC = '''#include <cdroid.h>
#include <cdlog.h>
#include "R.h"
#include "wind_home.h"

int main(int argc, const char* argv[]) {
    App app(argc, argv);
    cdroid::Context* ctx = &app;

    Window* w = new HomeWindow();
    return app.exec();
}

'''

writeFile(os.path.join(project_name,'main.cc'),mMainC);

mWindHomeH = '''#ifndef _WIND_HOME_H_
#define _WIND_HOME_H_
#include <cdroid.h>

class HomeWindow :public Window {
protected:
public:
    HomeWindow();
    ~HomeWindow();
private:

public:

};

#endif
'''

writeFile(os.path.join(project_name,'src','windows','wind_home.h'),mWindHomeH);

mWindHomeC = '''#include <wind_home.h>

HomeWindow::HomeWindow() : Window(0, 0, -1, -1) {
    LayoutInflater::from(getContext())->inflate("@layout/page_home", this);
}

HomeWindow::~HomeWindow() {
}
'''

writeFile(os.path.join(project_name,'src','windows','wind_home.cc'),mWindHomeC);


mWindHomeXML = '''<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/linearLayout"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="#000000">
    <TextView
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:text="HELLO WORLD"
        android:textSize="80dp"
        android:gravity="center"
        />
</LinearLayout>'''


writeFile(os.path.join(project_name,'assets','layout','page_home.xml'),mWindHomeXML);

mVersionH = '''#ifndef __CONF_VERSION_H__
#define __CONF_VERSION_H__

#include <stdio.h>
#include "hv_series_conf.h"

#define HV_SOFT_VER_MAJ "HV_MAJ"
#define HV_SOFT_VER_MIN "HV_MIN"

#define HV_SOFT_VER_EXT "RevTag"

#define HV_SOFT_VER_TIME "RevTIME"

#define HV_SOFT_GIT_HARD "RevGITLAB"

#define HV_SOFT_VERSION "V" HV_CPU_NAME "." HV_SOFT_VER_MAJ "." HV_SOFT_VER_MIN "." HV_SOFT_VER_EXT "." HV_SOFT_VER_TIME
#define HV_SOFT_VERSION_NEW HV_SOFT_VER_MAJ "." HV_SOFT_VER_EXT "." HV_SOFT_VER_MIN "." HV_SOFT_VER_TIME


#define HV_OS_VER_S "Linux 4.4"

#define HV_HARD_VER_S "RevHardVer"

#define HV_HARD_VERSION "V" HV_CPU_NAME "." HV_HARD_VER_S


/// @brief 获取格式化后的软件版本，用于展示
/// @param _szBuf 
/// @return 
static inline char* HV_FORMAT_VERSION_STRING(char* _szBuf)
{
	sprintf(_szBuf, "%s_V%s",HV_SERIES_NAME,HV_SOFT_VER_EXT);
	return _szBuf;
}

/// @brief 获取当前版本构建时间
/// @return 
static inline const char* HV_FORMAT_VER_TIME_STRING()
{
	return HV_SOFT_VER_TIME;
}

/// @brief 获取当前版本号
/// @return 
static inline const char* HV_FORMAT_VER_EXT_STRING()
{
	return HV_SOFT_VER_EXT;
}

static inline const char* HV_FORMAT_GIT_HARD_STRING()
{
	return HV_SOFT_GIT_HARD;
}

#endif

'''

writeFile(os.path.join(project_name,'conf','version.h'),mVersionH);

mDefaultConfig = '''#ifndef __DEFUALT_CONFIG_H__
#define __DEFUALT_CONFIG_H__

// 一些全局默认参数配置
#define  HV_SYS_CONFIG_SCREEN_BRIGHTNESS   50
#define  HV_SYS_CONFIG_MAX_SCREEN_BRIGHTNESS   100

#endif // __DEFUALT_CONFIG_H__'''

writeFile(os.path.join(project_name,'conf','defualt_config.h'),mDefaultConfig);


mCdroidSeries = '''#ifndef __HV_SERIES_CONFIG_H__
#define __HV_SERIES_CONFIG_H__

#define HV_SERIES_NAME	"CDROID"

#define HV_FLASH_SIZE	"128NR"
#define HV_FLASH_16NR	1

//cpu config
#define HV_CPU_NAME     "SSD202"
#define HV_CPU_BRAND    "Sigmstar"
#define HV_SIGM_SSD202   1

#define HV_FUNCTION_WIRE    1
#ifdef DEBUG
    #define HV_FUNCTION_WIFI    0
#else
    #define HV_FUNCTION_WIFI    1
#endif

/***********************************************/
#define HV_WLAN_NAME     "wlan0"

#define HV_WIRE_NAME     ""


#endif /*__HV_SERIES_CONFIG_H__*/
'''

writeFile(os.path.join(project_name,'conf','cdroid_series_conf.h'),mCdroidSeries);


mVSCodeConfig = """{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/*",
                "${workspaceFolder}/**",
                "~/cdroid/src/*",
                "~/cdroid/src/**",
                "/opt/vcpkg/packages/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}"""

writeFile(os.path.join(project_name,'.vscode','c_cpp_properties.json'),mVSCodeConfig);


mGitIgnore = """src/common/hv_version.h
src/common/hv_defualt_config.h
src/common/hv_series_conf.h
assets/values/ID.xml
R.h
"""

writeFile(os.path.join(project_name,'.gitignore'),mGitIgnore);

touch_file("CMakeLists.txt")
print(f"项目 '{project_name}' 初始化完成！")
