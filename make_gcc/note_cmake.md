## 一.常用命令

### 1 指定cmake最小版本
```
cmake_minimun_version(VERSION 3.4.1)
```

### 2 设置项目名称
```
project(demo)
```
该命令可选。会引入两个变量：
- demo_BINARY_DIR
- demo_SOURCE_DIR

同时，cmake自动定义了两个等价的变量PROJECT_BINARY_DIR和PROJECT_SOURCE_DIR.
备注：
- 1 CMAKE_BINART_DIR, PROJECT_BINARY_DIR, <projectName>_BINARY_DIR：这三个变量的含义一样。
 - 如果是in source编译：指的就是工程的顶层目录(CMakeLists.txt所在目录)
 - 如果是out source编译：指的就是工程编译发生的目录(build目录)
- 2 CMAKE_SOURCE_DIR, PROJECT_SOURCE_DIR, <projectName>_SOURCE_DIR：这三个变量的含义一样。都是指工程的顶级目录。

### 3 设置编译类型
```
//生成可执行文件
add_executable(demo demo.cpp)
//生成静态库(默认)
add_library(common STATIC util.cpp)
//生成动态库
add_library(common SHARED util.cpp)
```
add_library:默认生成的是静态库

### 4 制定编译包含的源文件
 - 1 明确制定包含的源文件
 ```
 add_library(demo demo.cpp test.cpp util.cpp)
 ```
 - 2 搜索所有cpp文件
 ```
 //发现一个目录下所有的源代码文件，并将列表存储再一个变量中(不会递归遍历目录)
aux_source_directory(. SRC_LIST)
add_library(demo ${SRC_LIST})
```
 - 3 自定义搜索规则
```
file(GLOB SRC_LIST "*.cpp" "protocol/*.cpp")
add_library(demo ${SRC_LIST})
#或者
file(GLOB SRC_LIST "*.cpp")
file(GLOB SRC_PROTOCOL_LIST "protocol/*.cpp")
add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})
#或者
aux_source_directory(. SRC_LIST)
aux_source_directory(protocol SRC_PROTOCOL_LIST)
add_library(demo ${SRC_LIST) ${SRC_PROTOCOL_LIST)
```

### 5 查找指定的库文件
```
//find_library(VAR name path)查找到指定的预编译库，并将它的路径存储在变量中。
//默认的搜索路径为cmake 包含的系统库，因此如果是NDK的公共库只需要指定库的name即可。
find_library(
                  #sets the name of the path variable
                  log-lib
                  #Specifies the name of the NDK library that you want CMake to locate
                  log
)
```
类似的命令：find_file(), find_path(), find_program(), find_package()

### 6 设置包含的目录
```
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
//linux下另一种方式设置：包含的目录
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")
```

### 7 设置链接库的搜索目录
```
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
//linux下另一种方式设置：链接的目录
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR)}/libs")
```

### 8 设置target需要连接的库
```
target_link_libraries(
    //目标库
    demo
    //目标库需要链接的库，log-lib是上面find_library指定的变量名
    ${log-lib}
)
```
linux下会搜索xxx.so或者xxx.a文件，如果都存在会优先链接动态库(.so)
 - 指定连接动态库或静态库
 ```
 target_link_libraries(demo libface.a)
target_link_libraries(demo libface.so)
```
 - 指定全路径
```
target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a)
target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.so)
```
 - 指定链接多个库
```
target_link_libraries(demo
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a
  boost_system.a
  boost_thread
  pthread
)
```

### 9 设置变量
 - 1 直接设置变量的值
```
set(SRC_LIST main.cpp test.cpp)
add_executable(demo ${SRC_LIST})
```
 - 2 追加设置变量的值
```
set(SRC_LIST main.cpp)
set(SRC_LIST ${SRC_LIST} test.cpp)
add_executable(demo ${SRC_LIST})
```
 - list 追加或者删除变量的值
```
set(SRC_LIST main.cpp)
list(append SRC_LIST test.cpp)
list(REMOVE_ITEM SRC_LIST main.cpp)
add_executable(demo ${SRC_LIST})
```

### 10 条件控制
 - 1 if...elseif...else...endif
```
//1.逻辑判断和比较
if(expression):expression 不为空.(0,N,NO,OFF,FALSE,NOTFOUND)时为真。
if(not exp):与上面相反
if(var1 AND var2)
if(var1 OR var2)
if(COMMAND cmd):如果cmd确实是命令，并且可调用为真
if(EXISTS dir)/ if(EXISTS):如果目录或文件存在为真
if(file1 IS_NEWER_THAN file2):当file1比file2新，或file1/file2中有一个不存在时为真，文件名需要使用全路径。
if(IS_DIRECTORY dir)：当dir是目录时为真
if(DEFINED var):如果变量被定义为真
if(var MATCHES regex):给定的变量或者字符串能够匹配正则表达式regex时为真，此处var可以用var命名，也可以用${var}
if(string MATCHES regex)
//2.数字比较
if(variable LESS number):LESS 小于
if(string LESS number)
if(variable GREATER number)
if(string GREATER number)
if(variable EQUAL number)
if(string EQUAL number)
//字母表顺序比较
if(variable STRLESS string)
if(string STRLESS string)
if (variable STRGREATER string)
if (string STRGREATER string)
if (variable STREQUAL string)
if (string STREQUAL string)
```
 - 2 while...endwhile
```
while(condition)
...
endwhile()
```
 - 3 foreach...endforeach
```
foreach(loop var RANGE start stop [step])
...
endforeach
```

### 11 打印信息
```
message(${PROJECT_SOURCE_DIR})
message("build with debug mode")
message(WARNING "this is warnning message")
message(FATAL_ERROR "this build has many error") #FATAL_ERROR会导致编译失败
```

### 12 包含其他cmake文件
```
//指定包含文件的全路径
include(./common.cmake)
//在搜索路径中搜索def.cmake文件
include(def)
//设置include的搜索路径
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
```

## 二.常用变量

### 1 预定义变量
```
PROJECT_SOURCE_DIR:工程根目录
PROJECT_BINARY_DIR:运行cmake命令的目录，通常是${PROJECT_SOURCE_DIR}/build
PROJECT_NAME:返回通过project命令定义的项目名称
CMAKE_CURRENT_SOURCE_DIR:当前处理的CMakeLists.txt所在的路径
CMAKE_CURRENT_BINARY_DIR:target 编译目录
CMAKE_CURRENT_LIST_DIR:CMakeLists.txt的完整路径
CMAKE_CURRENT_LIST_LINE:当前所在的行
CMAKE_MODULE_PATH:定义自己cmake模块所在的路径。SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)，然后可以用INCLUDE命令来调用自己的模块
EXECUTABLE_OUTPUT_PATH:重新定义目标二进制可执行文件的存放位置
LIBRARY_OUTPUT_PATH:重新定义目标链接库的存放位置
```

### 2 环境变量

 - 1 使用环境变量
 ```
$ENV{name}
 ```
 - 2 写入环境变量
```
set(ENV{name} value)
```


### 3 系统信息
```
CMAKE_MAJOR_VERSION:cmake主版本号(3.4.1:3)
CMAKE_MINIOR_VERSION:cmake次版本号(3.4.1:4)
CMAKE_PATCH_VERSION:cmake补丁等级(3.4.1:1)
CMAKE_SYSTEM:系统名称，比如Linux-2.6.22
CMAKE_SYSTEM_NAME:不包含版本的系统名，比如Linux
CMAKE_SYSTEM_VERSION:系统版本，比如2.6.22
CMAKE_SYSTEM_PROCESSOR:处理器名称，比如i686
UNIX:在所有的类UNIX平台下该值为TRUE，包括OS X和cygwin
WIN32:在所有的win32平台下该值为TRUE，包括cygwin
```

### 4 主要开关选项
```
BUILD_SHARED_LIBS:这个开关用来控制默认的库编译方式，如果不进行设置，使用add_library又没有指定库类型的情况下，默认编译生成的库都是静态库。如果set(BUILD_SHARED_LIBS ON)后，默认生成的是动态库。
CMAKE_C_FLAGS:设置C编译选项，也可以通过指令add_definitions()添加
CMAKE_CXX_FLAGS:设置C++编译选项，也可以通过指令add_definitions()添加。
```

##　三.常用复杂命令
### 1 CMake对文件的操作:file命令
```
//1.WRITE:写一条消息到名位filename中，如果文件存在，则会覆盖原文件，如果文件不存在，则创建该文件
file(WRITE filename "message to write" ...)
//2.APPEND:和WRITE选项一样，只是APPEND会写到文件的末尾
file(APPEND filename "message to write" ...)
//3. READ选项会将读取的文件内容存放到变量variable，读取numBytes个字节，从offset位置开始，如果制定了[HEX]参宿，二进制代码就会转成十六进制。
file(READ filename variable [LIMIT numBytes] [OFFSET offset] [HEX])
//4. STRINGS:该命令在变量myfile中存储了一个list，该list每一项是myfile.txt中的一行文本
file(STRINGS myfile.txt myfile)
//5. GLOB：该选项将会位所有匹配表达式的文件生成一个文件list，并将该list存放在variable里面，文件名的查询表达式和正则表达式类似
file(GLOB variable [RELATIVE path] [globbing expression])
//6.GLOB_RECURSE:会生成一个类似于通常GLOB选项的list,不过该选项可以递归查找文件中的匹配项
file(GLOB_RECURSE variable [RELATIVE path][FOLLOW_SYMLINKS] [globbing expressions])
//7.RENAME:对同一个文件系统下的一个文件或者目录重命名
file(RENAME <oldname> <newname>)
//8.REMOVE：将会删除指定的文件，包括在字路径下的文件
file(REMOVE [file1...])
//9.REMOVE_RECURSE:该选项会删除戈丁的文件以及目录，包括非空目录
file(REMOVE_RECURSE [file1 ...])
//10. MAKE_DIRECTORY:创建指定的目录，如果其父目录不存在时，同样也会创建
file(MAKE_DIRECTORY [directory1 directory2])
//11.RELATIVE_PATH:该选项会确定从directory参数到指定文件的相对路径，然后存到变量variable中
file(RELATIVE_PATH variable directory file)
//12. TO_CMAKE_PATH:该选项会把path转换位一个以unix的/开通的cmake风格的路径
file(TO_CMAKE_PATH path result)
//13.TO_NATIVE_PATH:把cmake风格的路径转换为本地路径的风格
file(TO_NATIVE_PATH path result)
//14.DOWNLOAD:该命令将给定的url下载到指定的文件中，如果指定了LOG,下载的日志将会被输出到log中；如果指定了STATUS status选项，下载操作的装填就会被输出到status里面，该状态的返回值是一个长度为2的字符串，错误信息如果是0，就表示没有错误；如果指定了TIMEOUT timeout选项，time秒之后，操作就会直接退出；如果指定了EXPECTED_MD5 sum选项，下载操作会认证下载的文件的实际MD5和是否与预期值相匹配，如果不匹配，操作将返回一个错误；如果指定SHOW_PROGRESS,进度信息会被打印出来，直到操作完成。
file(DOWNLOAD url file [TIMEOUT timeout] [STATUS status] [LOG log] [EXPECTED_MD5 sum] [SHOW_PROGRESS])
```